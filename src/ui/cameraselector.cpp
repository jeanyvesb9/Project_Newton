#include "cameraselector.h"
#include "ui_cameraselector.h"

CameraSelector::CameraSelector(CameraAnalyzerPointer *cam, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraSelector),
    cameraAnalyzer{cam}
{
    ui->setupUi(this);
    this->setModal(true);
    ui->resolutionList->setEnabled(false);

    cameraList = Camera::CameraInterface::getAvailableCameras();

    hadPreviousCameraOnInitialization = false;
    ui->cameraList->addItem(tr("None"));
    for(int i = 0; i < cameraList.size(); i++)
    {
        auto &c = cameraList.at(i);
        ui->cameraList->addItem(c.name);
        if(!cameraAnalyzer->isNull() && c.getId() == (*cameraAnalyzer)->getCameraDeviceInfo().getId())
        {
            hadPreviousCameraOnInitialization = true;
            on_cameraList_activated(i + 1);
            ui->cameraList->setCurrentIndex(i + 1);
        }
    }

    if(!hadPreviousCameraOnInitialization && cameraList.size() > 0)
    {
        on_cameraList_activated(1);
        ui->cameraList->setCurrentIndex(1);
    }
    else if(cameraList.size() == 0)
    {
        on_cameraList_activated(0);
        ui->cameraList->setCurrentIndex(0);
    }
    hadPreviousCameraOnInitialization = false;
}

CameraSelector::~CameraSelector()
{
    delete ui;
}

void CameraSelector::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QObject::disconnect(frameUpdateConnection);
    QObject::disconnect(errorConnection);
}

void CameraSelector::on_cameraList_activated(int index)
{
    ui->resolutionList->clear();
    resolutionList.clear();
    if(index == 0)
    {
        if(!cameraAnalyzer->isNull())
        {
            cameraAnalyzer->reset();
        }
        ui->cameraDisplay->setText(tr("Not Connected"));
        ui->resolutionList->setEnabled(false);
        ui->resolutionList->addItem(QStringLiteral("-"));
        ui->resolutionList->setCurrentIndex(0);
    }
    else
    {
        ui->resolutionList->setEnabled(true);

        if(!hadPreviousCameraOnInitialization)
        {
            (*cameraAnalyzer) = CameraAnalyzerPointer(new CameraAnalyzer(cameraList.at(index - 1)));
        }
        resolutionList = (*cameraAnalyzer)->getCameraResolutions();
        for(int i = 0; i < resolutionList.size(); i++)
        {
            auto &s = resolutionList.at(i);
            ui->resolutionList->addItem(s.description);
            if(s == (*cameraAnalyzer)->getCurrentResolution())
            {
                ui->resolutionList->setCurrentIndex(i);
            }
        }
        (*cameraAnalyzer)->startCapture();

        frameUpdateConnection = QObject::connect(cameraAnalyzer->data(), &CameraAnalyzer::rawFromCamera, this, [this](QImage img) {
            this->ui->cameraDisplay->setPixmap(QPixmap::fromImage(img).scaled(this->ui->cameraDisplay->width(), this->ui->cameraDisplay->height(), Qt::KeepAspectRatio));
        });
        errorConnection = QObject::connect(cameraAnalyzer->data(), &CameraAnalyzer::cameraError, this, [this](QString errorString) {
            QMessageBox msgBox;
            msgBox.setText(errorString);
            msgBox.exec();
            this->close();
        });
    }
}

void CameraSelector::on_resolutionList_activated(int index)
{
    if(!cameraAnalyzer->isNull())
    {
        (*cameraAnalyzer)->setCameraResolution(resolutionList.at(index));
    }
}

void CameraSelector::on_closeBtn_clicked()
{
    close();
}

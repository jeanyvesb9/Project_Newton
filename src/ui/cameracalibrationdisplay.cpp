#include "cameracalibrationdisplay.h"
#include "ui_cameracalibrationdisplay.h"

CameraCalibrationDisplay::CameraCalibrationDisplay(CameraAnalyzerPointer cam, bool newCal, bool errorHandling, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraCalibrationDisplay),
    existingCalFlag{!newCal}, camera{cam}, errorHandlingResponsibility{errorHandling}
{
    ui->setupUi(this);

    ui->cameraName->setText(camera->getCameraDeviceInfo().name);
    resolutionList = camera->getCameraResolutions();
    const Camera::ResolutionInfo activeRes = camera->getCurrentResolution();
    for(int i = 0; i < resolutionList.size(); i++)
    {
        auto &s = resolutionList.at(i);
        ui->resolutionList->addItem(s.description);
        if(s == activeRes)
        {
            ui->resolutionList->setCurrentIndex(i);
        }
    }

    ui->color_spinBox->setRange(0, 2000);
    ui->color_slider->setRange(0, 2000);
    int lRange = 0, hRange = 1000;
    ui->chlt_spinBox->setRange(lRange, hRange);
    ui->chlt_slider->setRange(lRange, hRange);
    ui->chut_spinBox->setRange(lRange, hRange);
    ui->chut_slider->setRange(lRange, hRange);
    trimDisp = dynamic_cast<TrimDisplay *>(ui->cameraTrim);

    rawFromCameraConnection = QObject::connect(camera.data(), &CameraAnalyzer::rawFromCamera, [this](QImage img) {
        this->trimDisp->newFrame(img);
        if(this->existingCalFlag)
        {
            QVector<QPoint> corners = camera->getTrimPoints();
            this->trimDisp->setCorners(corners.at(0), corners.at(1), corners.at(2), corners.at(3));
            this->existingCalFlag = false;
        }
    });
    pieceOverlayConnection = QObject::connect(camera.data(), &CameraAnalyzer::pieceOverlay, [this](QImage img) {
        ui->pieceOverlay_videoFeed->setPixmap(QPixmap::fromImage(img).scaled(ui->pieceOverlay_videoFeed->width(), ui->pieceOverlay_videoFeed->height(), Qt::KeepAspectRatio));
    });
    edgeViewConnection = QObject::connect(camera.data(), &CameraAnalyzer::edgeView, [this](QImage img) {
        ui->edgesOverlay_videoFeed->setPixmap(QPixmap::fromImage(img).scaled(ui->edgesOverlay_videoFeed->width(), ui->edgesOverlay_videoFeed->height(), Qt::KeepAspectRatio));
    });
    if(newCal)
    {
        QObject::connect(trimDisp, &TrimDisplay::cornersMoved, camera.data(), &CameraAnalyzer::setTrimPoints);
    }
    else
    {
        trimPointsFirstMoved = QObject::connect(trimDisp, &TrimDisplay::cornersMoved, this, [this](QPoint ul, QPoint ur, QPoint bl, QPoint br) {
            QVector<QPoint> corners = this->camera->getTrimPoints();
            if(corners.at(0) == ul && corners.at(1) == ur && corners.at(2) == bl && corners.at(3) == br)
            {
                QObject::disconnect(this->trimPointsFirstMoved);
                QObject::connect(this->trimDisp, &TrimDisplay::cornersMoved, this->camera.data(), &CameraAnalyzer::setTrimPoints);
            }
        });
    }
    errorConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &CameraCalibrationDisplay::cameraError);

    if(newCal)
    {
        on_defaultsBtn_clicked();
    }
    else
    {
        ui->color_spinBox->setValue(camera->getColorThreshold());
        ui->chlt_spinBox->setValue(camera->getCannyHysterisisLThold());
        ui->chut_spinBox->setValue(camera->getCannyHysterisisUThold());
    }
}

CameraCalibrationDisplay::~CameraCalibrationDisplay()
{
    delete ui;
}

void CameraCalibrationDisplay::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QObject::disconnect(rawFromCameraConnection);
    QObject::disconnect(pieceOverlayConnection);
    QObject::disconnect(edgeViewConnection);
    QObject::disconnect(errorConnection);
    emit closed();
}

void CameraCalibrationDisplay::on_color_slider_sliderMoved(int position)
{
    ui->color_spinBox->setValue(position);
}

void CameraCalibrationDisplay::on_chlt_slider_sliderMoved(int position)
{
    ui->chlt_spinBox->setValue(position);
}

void CameraCalibrationDisplay::on_chut_slider_sliderMoved(int position)
{
    ui->chut_spinBox->setValue(position);
}

void CameraCalibrationDisplay::on_color_spinBox_valueChanged(int arg1)
{
    ui->color_slider->setValue(arg1);
    camera->setColorThreshold(arg1);
}

void CameraCalibrationDisplay::on_chlt_spinBox_valueChanged(int arg1)
{
    ui->chlt_slider->setValue(arg1);
    camera->setCannyHysterisisLThold(arg1);
}

void CameraCalibrationDisplay::on_chut_spinBox_valueChanged(int arg1)
{
    ui->chut_slider->setValue(arg1);
    camera->setCannyHysterisisUThold(arg1);
}

void CameraCalibrationDisplay::on_defaultsBtn_clicked()
{
    ui->color_spinBox->setValue(700);
    ui->chlt_spinBox->setValue(50); //100
    ui->chut_spinBox->setValue(70); //105
}

void CameraCalibrationDisplay::on_resolutionList_activated(int index)
{
    camera->setCameraResolution(resolutionList.at(index));
}

void CameraCalibrationDisplay::cameraError(QString error)
{
    if(errorHandlingResponsibility)
    {
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
        this->close();
    }
}

bool CameraCalibrationDisplay::getErrorHandlingResponsibility() const
{
    return errorHandlingResponsibility;
}

void CameraCalibrationDisplay::setErrorHandlingResponsibility(bool value)
{
    errorHandlingResponsibility = value;
}

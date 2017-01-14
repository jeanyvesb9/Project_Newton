#ifndef CAMERASELECTOR_H
#define CAMERASELECTOR_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "src/camera/cameraanalyzer.h"

namespace Ui {
class CameraSelector;
}

class CameraSelector : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSelector(CameraAnalyzerPointer *cam, QWidget *parent = 0);
    ~CameraSelector();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_cameraList_activated(int index);
    void on_resolutionList_activated(int index);
    void on_closeBtn_clicked();

private:
    Ui::CameraSelector *ui;
    QMetaObject::Connection frameUpdateConnection;
    QMetaObject::Connection errorConnection;
    bool hadPreviousCameraOnInitialization;

    Camera::CameraDeviceInfoList cameraList;
    Camera::ResolutionInfoList resolutionList;
    CameraAnalyzerPointer *cameraAnalyzer;
};

#endif // CAMERASELECTOR_H

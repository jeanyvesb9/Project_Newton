#ifndef CAMERACALIBRATIONDISPLAY_H
#define CAMERACALIBRATIONDISPLAY_H

#include <QMainWindow>
#include <QImage>
#include <QVector>
#include <QDebug>
#include <QMessageBox>

#include "src/ui/trimdisplay.h"
#include "src/camera/cameraanalyzer.h"

namespace Ui {
class CameraCalibrationDisplay;
}

class CameraCalibrationDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraCalibrationDisplay(CameraAnalyzerPointer cam, bool existingCalFlag, bool errorHandling, QWidget *parent = 0);
    ~CameraCalibrationDisplay();

    bool getErrorHandlingResponsibility() const;
    void setErrorHandlingResponsibility(bool value);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_color_slider_sliderMoved(int position);
    void on_chlt_slider_sliderMoved(int position);
    void on_chut_slider_sliderMoved(int position);
    void on_color_spinBox_valueChanged(int arg1);
    void on_chlt_spinBox_valueChanged(int arg1);
    void on_chut_spinBox_valueChanged(int arg1);
    void on_defaultsBtn_clicked();
    void on_resolutionList_activated(int index);

    void cameraError(QString error);

private:
    Ui::CameraCalibrationDisplay *ui;
    TrimDisplay *trimDisp;
    bool existingCalFlag;
    QMetaObject::Connection rawFromCameraConnection;
    QMetaObject::Connection pieceOverlayConnection;
    QMetaObject::Connection edgeViewConnection;
    QMetaObject::Connection trimPointsFirstMoved;
    QMetaObject::Connection errorConnection;

    CameraAnalyzerPointer camera;
    Camera::ResolutionInfoList resolutionList;

    bool errorHandlingResponsibility;
};

#endif // CAMERACALIBRATIONDISPLAY_H

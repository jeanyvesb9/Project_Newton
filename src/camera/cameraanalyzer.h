#ifndef CAMERAANALYZER_H
#define CAMERAANALYZER_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QImage>
#include <QPixmap>
#include <QPoint>
#include <math.h>
#include <vector>
#include <opencv/cv.hpp>

#include "src/game/board.h"
#include "src/camera/camerainterface.h"

enum BoardPiece { PlayerPiece, OpPlayerPiece, Empty };
QVector<BoardPiece> convertBoardToBoardPieceVector(Game::BoardPointer board);

class CameraAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit CameraAnalyzer(Camera::CameraDeviceInfo cam, QObject *parent = 0);
    ~CameraAnalyzer();

    const Camera::CameraDeviceInfo getCameraDeviceInfo() const;
    const Camera::ResolutionInfo getCurrentResolution() const;
    const Camera::ResolutionInfoList getCameraResolutions() const;
    QVector<QPoint> getTrimPoints() const;
    int getColorThreshold() const;
    int getCannyHysterisisLThold() const;
    int getCannyHysterisisUThold() const;
    QImage getLastRawFromCamera();
    QImage getLastPieceOverlay();
    QImage getLastEdgeView();
    int getFramerate() const;
    bool getErrorState() const;

signals:
    void cameraError(QString error);

    void rawFromCamera(QImage img);
    void pieceOverlay(QImage img);
    void edgeView(QImage img);

public slots:
    bool startCapture();
    bool stopCapture();
    void setFramerate(int value);
    void setTrimPoints(QPoint ul, QPoint ur, QPoint bl, QPoint br);
    void setCameraResolution(const Camera::ResolutionInfo res);
    void setColorThreshold(int value);
    void setCannyHysterisisLThold(int value);
    void setCannyHysterisisUThold(int value);

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void processImage(QImage img);

private:
    int timerId;
    int framerate;
    bool errorState;

    Camera::CameraInterface *camera;
    Camera::CameraDeviceInfo cameraInfo;

    QImage lastRawFromCamera;
    QImage lastPieceOverlay;
    QImage lastEdgeView;
    QVector<BoardPiece> rawBoard;

    QPoint ul;
    QPoint ur;
    QPoint bl;
    QPoint br;
    int colorThreshold;
    int CannyHysterisisLThold;
    int CannyHysterisisUThold;

    BoardPiece searchCell(cv::Mat cell);

    QImage Mat2QImage_Ref(cv::Mat &mat, bool swap = true);
    QImage Mat2QImage(cv::Mat const &mat, bool swap = true);
    cv::Mat QImage2Mat_Ref(QImage &img, bool swap = true);
    cv::Mat QImage2Mat(QImage const &img, bool swap = true);
};

using CameraAnalyzerPointer = QSharedPointer<CameraAnalyzer>;

#endif // CAMERAANALYZER_H

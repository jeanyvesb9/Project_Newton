#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QDebug>

#ifdef WIN32
#include <videoInput/videoInput.h>
#include <opencv/cv.hpp>
#endif


namespace Camera
{

class CameraInterface;

template<typename T> struct _Point
{
    _Point() :x{0}, y{0} {}
    _Point(T x, T y) :x{x}, y{y} {}
    T x;
    T y;
    inline bool operator==(const _Point &other) const { return x == other.x && y == other.y; }
};

using Point = _Point<int>;
using PointF = _Point<float>;
using PointD = _Point<double>;

using Resolution = _Point<unsigned int>;
struct ResolutionInfo
{
    ResolutionInfo() = default;
    ResolutionInfo(Resolution res) : res{res} { description = QString::number(res.x) + QStringLiteral("x") + QString::number(res.y); }
    ResolutionInfo(Resolution res, QString apendedDescription) : res{res} { description = QString::number(res.x) + QStringLiteral("x") + QString::number(res.y) + QStringLiteral(" - ") + apendedDescription; }
    Resolution res;
    QString description;
    inline bool operator==(const ResolutionInfo &other) const { return id == other.id; }
private:
    quint64 id;
    ResolutionInfo(Resolution res, QString apendedDescription, quint64 id) : res{res}, id{id} { description = QString::number(res.x) + QStringLiteral("x") + QString::number(res.y) + (apendedDescription.isEmpty() ? QStringLiteral("") : QStringLiteral(" - ") + apendedDescription); }
    friend class CameraInterface;
};
using ResolutionInfoList = QVector<ResolutionInfo>;

struct CameraDeviceInfo
{
    CameraDeviceInfo() = default;
    QString name;
    inline const QVariant getId() const { return id; }
private:
    CameraDeviceInfo(QString name, QVariant id) : name{name}, id{id} {;}
    QVariant id;
    friend class CameraInterface;
};
using CameraDeviceInfoList = QVector<CameraDeviceInfo>;

#ifdef WIN32
class CamStopCallback;
#endif

class CameraInterface : public QObject
{
    Q_OBJECT
public:
    explicit CameraInterface(CameraDeviceInfo cam, QObject *parent = 0);
    ~CameraInterface();

    const ResolutionInfoList getCameraResolutions() const;
    const ResolutionInfo getCurrentResolution() const;
    void setCameraResolution(const ResolutionInfo res);

    static const CameraDeviceInfoList getAvailableCameras();

signals:
    void hasInitialized();
    void hasCapturedImage(QImage img);
    void cameraError(QString error);

public slots:
    bool captureImage();

private:
    bool isCapturing;
    ResolutionInfo currentResolution;

#ifdef MACOS
    struct Private;
    Private *p;

#endif
#ifdef WIN32
    ResolutionInfoList resolutionList;
    CamStopCallback *callback;
    DeviceSettings deviceSettings;
    ReadSetting readSetting;
    IplImage *frame;

    void startDevice();
    QImage IplImageToQImage(const IplImage *iplImage) const;
#endif
#ifdef LINUX

#endif

};

#ifdef WIN32
class CamStopCallback : public IStopCallback
{
public:
    CamStopCallback(CameraInterface *interface);
    void Invoke(StopCallbackEvent::CallbackEvent callbackEvent) override;
private:
    CameraInterface *interface;
};
#endif

}
#endif // CAMERAINTERFACE_H

#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QDebug>

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
    ResolutionInfo(Resolution res, QString apendedDescription, quint64 id) : res{res}, id{id} { description = QString::number(res.x) + QStringLiteral("x") + QString::number(res.y) + QStringLiteral(" - ") + apendedDescription; }
    friend class CameraInterface;
};
using ResolutionInfoList = QVector<ResolutionInfo>;

struct CameraDeviceInfo
{
    QString name;
    inline const QVariant getId() const { return id; }
private:
    QVariant id;
    friend class CameraInterface;
};
using CameraDeviceInfoList = QVector<CameraDeviceInfo>;

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
#ifdef WINDOWS

#endif
#ifdef LINUX

#endif

};

}
#endif // CAMERAINTERFACE_H

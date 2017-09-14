#include "camerainterface.h"

#ifndef MACOS
#ifdef WIN32

Camera::CameraInterface::CameraInterface(Camera::CameraDeviceInfo cam, QObject *parent)
    :QObject(parent), isCapturing{false}, callback{new CamStopCallback(this)}, frame{nullptr}
{
    deviceSettings.symbolicLink = cam.getId().toString().toStdWString();
    deviceSettings.indexStream = 0;

    std::vector<Device> devices;
    videoInput::getInstance().getListOfDevices(devices);
    for(auto &dev : devices)
    {
        if(dev.symbolicName == deviceSettings.symbolicLink)
        {
            vector<MediaType> mTypes = dev.listStream[0].listMediaType;
            for(int i = 0; i < mTypes.size(); i++)
            {
                resolutionList <<ResolutionInfo({mTypes.at(i).width, mTypes.at(i).height}, QStringLiteral(""), i);
            }
            currentResolution = resolutionList.at(0);
            break;
        }
    }

    startDevice();
}

Camera::CameraInterface::~CameraInterface()
{
    ResultCode::Result result = videoInput::getInstance().closeDevice(deviceSettings);
    if(callback)
    {
        delete callback;
    }
    if(result != ResultCode::OK)
    {
        //TODO: RAISE EXCEPTION
    }
}

const Camera::ResolutionInfoList Camera::CameraInterface::getCameraResolutions() const
{
    return resolutionList;
}

const Camera::ResolutionInfo Camera::CameraInterface::getCurrentResolution() const
{
    return currentResolution;
}

void Camera::CameraInterface::setCameraResolution(const Camera::ResolutionInfo res)
{
    ResultCode::Result result = videoInput::getInstance().closeDevice(deviceSettings);
    if(result != ResultCode::OK)
    {
        //TODO: RAISE EXCEPTION
        return;
    }
    currentResolution = res;
    startDevice();
}

const Camera::CameraDeviceInfoList Camera::CameraInterface::getAvailableCameras()
{    
    CameraDeviceInfoList ret;

    std::vector<Device> devices;
    ResultCode::Result result = videoInput::getInstance().getListOfDevices(devices);
    if(result == ResultCode::OK)
    {
        for(int i = 0; i < devices.size(); i++)
        {
            auto &dev = devices.at(i);
            ret << CameraDeviceInfo(QString::fromStdWString(dev.friendlyName), QString::fromStdWString(dev.symbolicName));
        }
    }
    else
    {
        //TODO: RAISE EXCEPTION
    }

    return ret;
}

bool Camera::CameraInterface::captureImage()
{
    ResultCode::Result result = videoInput::getInstance().readPixels(readSetting);
    if(result == ResultCode::READINGPIXELS_DONE)
    {
        emit this->hasCapturedImage(IplImageToQImage(frame));
        return true;
    }
    return false;
}

void Camera::CameraInterface::startDevice()
{
    if(frame)
    {
        cvReleaseImage(&frame);
    }
    frame = cvCreateImage(cvSize(currentResolution.res.x, currentResolution.res.y), 8, 4);
    readSetting.symbolicLink = deviceSettings.symbolicLink;
    readSetting.pPixels = reinterpret_cast<uchar*>(frame->imageData);
    deviceSettings.indexMediaType = currentResolution.id;
    CaptureSettings capSettings;
    capSettings.pIStopCallback;
    capSettings.readMode = ReadMode::ASYNC;
    capSettings.videoFormat = CaptureVideoFormat::RGB32;

    ResultCode::Result result = videoInput::getInstance().setupDevice(deviceSettings, capSettings);
    if(result != ResultCode::OK)
    {
        //TODO: RAISE EXCEPTION
    }
}

QImage Camera::CameraInterface::IplImageToQImage(const IplImage *iplImage) const
{
    const uchar *qImageBuffer = reinterpret_cast<const uchar*>(iplImage->imageData);//(const uchar*)iplImage->imageData;
    QImage img(qImageBuffer, iplImage->width, iplImage->height, QImage::Format_RGBA8888);
    return img.rgbSwapped();
}

Camera::CamStopCallback::CamStopCallback(Camera::CameraInterface *interface)
    : interface{interface}
{

}

void Camera::CamStopCallback::Invoke(StopCallbackEvent::CallbackEvent callbackEvent)
{
    Q_UNUSED(callbackEvent)
    emit interface->cameraError(QStringLiteral("Camera Disconected"));
}

#endif
#endif

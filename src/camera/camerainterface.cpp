#include "camerainterface.h"

#ifndef MACOS

Camera::CameraInterface::CameraInterface(Camera::CameraDevice cam, QObject *parent)
    :QObject(parent)
{

}

Camera::CameraInterface::~CameraInterface()
{

}

const Camera::ResolutionList Camera::CameraInterface::getCameraResolutions() const
{

}

void Camera::CameraInterface::setCameraResolution(Camera::Resolution)
{

}

const Camera::CameraDeviceList Camera::CameraInterface::getAvailableCameras()
{

}

bool Camera::CameraInterface::captureImage()
{

}

#endif

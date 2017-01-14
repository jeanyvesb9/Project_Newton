#include "camerainterface.h"

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>

@interface CameraNotificationHandler : NSObject

@property (assign) Camera::CameraInterface *interface;

- (void) setupNotificationFrom:(AVCaptureDevice *)device;
- (void) receiveCameraDisconnectionNotification:(NSNotification *)notification;

@end

@implementation CameraNotificationHandler

- (id) init
{
    self = [super init];
    if(!self) return nil;

    return self;
}

- (void) dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}

- (void) setupNotificationFrom:(AVCaptureDevice *)device
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(receiveCameraDisconnectionNotification:) name:AVCaptureDeviceWasDisconnectedNotification object:device];
}

- (void) receiveCameraDisconnectionNotification:(NSNotification *)notification
{
    if(_interface)
    {
        emit _interface->cameraError(QString::fromNSString([notification.object localizedName]));
    }
}

@end

struct Camera::CameraInterface::Private
{
    AVCaptureDevice *activeDevice = nil;
    AVCaptureSession *captureSession = nil;
    AVCaptureStillImageOutput *imageOutput = nil;
    NSMutableArray *resolutions = nil;
    CameraNotificationHandler *notificationHandler = nil;

    ~Private()
    {
        activeDevice = nil;
        captureSession = nil;
        imageOutput = nil;
        resolutions = nil;
        notificationHandler = nil;
    }
};

Camera::CameraInterface::CameraInterface(Camera::CameraDeviceInfo cam, QObject *parent)
    :QObject(parent), isCapturing{false}, p{new CameraInterface::Private}
{
    p->activeDevice = [AVCaptureDevice deviceWithUniqueID:cam.id.toString().toNSString()];

    p->notificationHandler = [[CameraNotificationHandler alloc] init];
    p->notificationHandler.interface = this;
    [p->notificationHandler setupNotificationFrom:p->activeDevice];

    p->captureSession = [[AVCaptureSession alloc] init];

    NSError *error;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:p->activeDevice error:&error];
    if(p->captureSession && input && [p->captureSession canAddInput: input])
    {
        [p->captureSession addInput:input];
        p->captureSession.sessionPreset = AVCaptureSessionPresetPhoto;

        const ResolutionInfoList resList = getCameraResolutions();
        bool flag = false;
        const Resolution target(640, 480);
        for(const auto &r : resList)
        {
            if(r.res == target)
            {
                flag = true;
                NSError *error = nil;
                if([p->activeDevice lockForConfiguration:&error])
                {
                    p->activeDevice.activeFormat = p->resolutions[r.id];
                    currentResolution = r;
                    [p->activeDevice unlockForConfiguration];
                    break;
                }
                else
                {
                    qDebug() <<"ERROR";//TODO: RAISE EXCEPTION
                }
            }
        }
        if(!flag)
        {
            NSError *error = nil;
            if([p->activeDevice lockForConfiguration:&error])
            {
                const ResolutionInfo &res = resList.at(0);
                p->activeDevice.activeFormat = p->resolutions[res.id];
                currentResolution = res;
                [p->activeDevice unlockForConfiguration];
            }
            else
            {
                //TODO: RAISE EXCEPTION
            }
        }
    }
    else
    {
        //TODO: RAISE EXCEPTION
    }

    p->imageOutput = [[AVCaptureStillImageOutput alloc] init];
    [p->imageOutput setOutputSettings:@{ AVVideoCodecKey : AVVideoCodecJPEG}];
    if([p->captureSession canAddOutput:p->imageOutput])
    {
        [p->captureSession addOutput:p->imageOutput];
    }
    else
    {
        //TODO: RAISE EXCEPTION
    }

    [p->captureSession startRunning];
    emit hasInitialized();
}

Camera::CameraInterface::~CameraInterface()
{
    if(isCapturing)
    {
        //TODO: Wait for closure;
    }
    if(p->captureSession && p->captureSession.isRunning)
    {
        [p->captureSession stopRunning];
    }
    delete p;
}

const Camera::ResolutionInfoList Camera::CameraInterface::getCameraResolutions() const
{
    ResolutionInfoList retList;
    if(p->resolutions)
    {
        [p->resolutions removeAllObjects];
    }
    else
    {
        p->resolutions = [[NSMutableArray alloc] init];
    }
    NSArray *resolutions = p->activeDevice.formats;
    for(unsigned int i = 0; i < resolutions.count; i++)
    {
        AVCaptureDeviceFormat *format = resolutions[i];
        [p->resolutions addObject:format];
        CMVideoDimensions dim = CMVideoFormatDescriptionGetDimensions(format.formatDescription);
        CFStringRef str = CFStringCreateWithCString(kCFAllocatorDefault, "FormatName", kCFStringEncodingUTF8);
        NSString *encoding = (__bridge NSString *)CMFormatDescriptionGetExtension(format.formatDescription, str);
        retList << ResolutionInfo(Resolution(dim.width, dim.height), QString::fromNSString(encoding), i);
        CFRelease(str);
    }

    return retList;
}

const Camera::ResolutionInfo Camera::CameraInterface::getCurrentResolution() const
{
    return currentResolution;
}

void Camera::CameraInterface::setCameraResolution(const ResolutionInfo res)
{
    if(p->activeDevice != nil && p->resolutions != nil && res.id < p->resolutions.count)
    {
        NSError *error = nil;
        if([p->activeDevice lockForConfiguration:&error])
        {
            p->activeDevice.activeFormat = p->resolutions[res.id];
            [p->activeDevice unlockForConfiguration];
        }
        else
        {
            //TODO: RAISE EXCEPTION
        }
    }
}

const Camera::CameraDeviceInfoList Camera::CameraInterface::getAvailableCameras()
{
    CameraDeviceInfoList ret;

    NSArray *devicesArray = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];

    for(unsigned int i = 0; i < devicesArray.count; i++)
    {
        AVCaptureDevice *device = devicesArray[i];
        CameraDeviceInfo dev;
        dev.name = QString::fromNSString(device.localizedName);
        dev.id = QString::fromNSString(device.uniqueID);
        ret << dev;
    }

    return ret;
}

bool Camera::CameraInterface::captureImage()
{
    if(p->imageOutput && !isCapturing)
    {
        isCapturing = true;

        AVCaptureConnection *videoConnection = nil;
        for(AVCaptureConnection *connection in p->imageOutput.connections)
        {
            for(AVCaptureInputPort *port in [connection inputPorts])
            {
                if([[port mediaType] isEqual:AVMediaTypeVideo])
                {
                    videoConnection = connection;
                    break;
                }
            }
            if(videoConnection)
            {
                break;
            }
        }

        [p->imageOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler:^(CMSampleBufferRef imageSampleBuffer, NSError *error) {
            if(error)
            {
                emit this->cameraError(QString::fromNSString(error.localizedDescription));
                return;
            }

            NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageSampleBuffer];
            QImage img = QImage::fromData(static_cast<const unsigned char *>(imageData.bytes), imageData.length, "JPEG");
            emit this->hasCapturedImage(img);

            this->isCapturing = false;
        }];
        return true;
    }
    return false;
}

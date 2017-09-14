#include "cameraanalyzer.h"

QVector<BoardPiece> convertBoardToBoardPieceVector(Game::BoardPointer board)
{
    QVector<BoardPiece> ret;
    for(auto &d : board->getBoardData())
    {
        switch(d)
        {
        case Game::Piece::Player:
        case Game::Piece::King:
            ret << BoardPiece::PlayerPiece;
            break;
        case Game::Piece::OpPlayer:
        case Game::Piece::OpKing:
            ret << BoardPiece::OpPlayerPiece;
            break;
        case Game::Piece::Empty:
            ret << BoardPiece::Empty;
            break;
        }
    }
    return ret;
}

//Helper FN END --------------------------------------------------------------------------------------------

CameraAnalyzer::CameraAnalyzer(Camera::CameraDeviceInfo cam, QObject *parent)
    : QObject(parent),
      timerId{0}, framerate{5}, errorState{false}, camera{new Camera::CameraInterface(cam)}, cameraInfo{cam}, boardCaptureRequest{false}
{
    QObject::connect(camera, &Camera::CameraInterface::cameraError, this, [this](QString error) {
        errorState = true;
        this->stopCapture();
        emit this->cameraError(error);
    });
    QObject::connect(camera, &Camera::CameraInterface::hasCapturedImage, this, &CameraAnalyzer::processImage);
}

CameraAnalyzer::~CameraAnalyzer()
{
    if(camera)
    {
        delete camera;
    }
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
}

const Camera::CameraDeviceInfo CameraAnalyzer::getCameraDeviceInfo() const
{
    return cameraInfo;
}

const Camera::ResolutionInfo CameraAnalyzer::getCurrentResolution() const
{
    return camera->getCurrentResolution();
}

const Camera::ResolutionInfoList CameraAnalyzer::getCameraResolutions() const
{
    return camera->getCameraResolutions();
}

QVector<QPoint> CameraAnalyzer::getTrimPoints() const
{
    QVector<QPoint> ret = {ul, ur, bl, br};
    return ret;
}

void CameraAnalyzer::setTrimPoints(QPoint ul, QPoint ur, QPoint bl, QPoint br)
{
    this->ul = ul;
    this->ur = ur;
    this->bl = bl;
    this->br = br;
}

void CameraAnalyzer::setCameraResolution(const Camera::ResolutionInfo res)
{
    camera->setCameraResolution(res);
}

void CameraAnalyzer::processImage(QImage img)
{
    //IMAGE SPLIT -------------------------------------------------
    //Original Image Rotation
    cv::Mat originalImg;
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2f(img.width() / 2, img.height() / 2), 180, 1);
    cv::warpAffine(QImage2Mat(img), originalImg, rotationMatrix, cv::Size(img.width(), img.height()));

    //Trimming:
    cv::Point2f inputPts[4] = { cv::Point2f(ul.x(), ul.y()),
                                cv::Point2f(ur.x(), ur.y()),
                                cv::Point2f(bl.x(), bl.y()),
                                cv::Point2f(br.x(), br.y()) };
    cv::Point2f outputPts[4] = { cv::Point2f(0, 0),
                                 cv::Point2f(400, 0),
                                 cv::Point2f(0, 400),
                                 cv::Point2f(400, 400) };
    cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(inputPts, outputPts);
    cv::Mat trimmedImg = originalImg.clone();
    cv::warpPerspective(trimmedImg, trimmedImg, perspectiveMatrix, cv::Size(400, 400));

    //Rotation:
    //cv::Mat rotationMatrix = cv::getRotationMatrix2D(cv::Point2f(trimmedImg.size().width / 2, trimmedImg.size().height / 2), 180, 1);
    //cv::warpAffine(trimmedImg, trimmedImg, rotationMatrix, trimmedImg.size());

    //Edge Image Generation
    cv::Mat edgesImg;
    cv::cvtColor(trimmedImg, edgesImg, cv::COLOR_BGRA2GRAY);
    cv::medianBlur(edgesImg, edgesImg, 5);
    cv::GaussianBlur(edgesImg, edgesImg, cv::Size(3, 3), 0);
    cv::Canny(edgesImg, edgesImg, CannyHysterisisLThold, CannyHysterisisUThold);

    QMap<QPair<int, int>, QPoint> cornerPoints;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            cornerPoints[{i, j}] = QPoint(i == 400 ? 399 : i * 50, j == 400 ? 399 : j * 50);
        }
    }

    QMap<QPair<int, int>, cv::Mat> fieldTable;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int x1 = cornerPoints.value({i, j}).x();
            int y1 = cornerPoints.value({i, j}).y();
            int x2 = cornerPoints.value({i + 1, j + 1}).x();
            int y2 = cornerPoints.value({i + 1, j + 1}).y();
            fieldTable[{i, j}] = cv::Mat(trimmedImg, cv::Range(y1, y2), cv::Range(x1, x2));
        }
    }

    //Cells scan
    QVector<BoardPiece> rawBoard;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
            {
                rawBoard << searchCell(fieldTable.value({j, i}));
            }
        }
    }

    //Feducial markings
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int x = cornerPoints.value({i, j}).x();
            int y = cornerPoints.value({i, j}).y();
            cv::circle(trimmedImg, cv::Point(x, y), 4, cv::Scalar(0x00, 0x00, 0xFF, 0xFF));
        }
    }

    lastRawFromCamera = Mat2QImage(originalImg);
    lastPieceOverlay = Mat2QImage(trimmedImg);
    lastEdgeView = Mat2QImage(edgesImg);
    emit rawFromCamera(lastRawFromCamera);
    emit pieceOverlay(lastPieceOverlay);
    emit edgeView(lastEdgeView);
    if(boardCaptureRequest)
    {
        emit rawBoardVector(rawBoard);
    }
}

bool CameraAnalyzer::getErrorState() const
{
    return errorState;
}

int CameraAnalyzer::getFramerate() const
{
    return framerate;
}

void CameraAnalyzer::setFramerate(int value)
{
    framerate = value;
    if(timerId)
    {
        killTimer(timerId);
        timerId = startTimer(1000 / framerate);
    }
}

BoardPiece CameraAnalyzer::searchCell(cv::Mat cell)
{
    cv::Mat cellGrayscale;
    cv::Mat cellEdges;
    cv::cvtColor(cell, cellGrayscale, cv::COLOR_BGRA2GRAY);
    cv::medianBlur(cellGrayscale, cellGrayscale, 5);
    cv::GaussianBlur(cellGrayscale, cellEdges, cv::Size(3, 3), 0);
    cv::Canny(cellEdges, cellEdges, CannyHysterisisLThold, CannyHysterisisUThold);
    cv::Mat dst;
    cv::threshold(cellGrayscale, dst, 50, 255, cv::THRESH_BINARY);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(cellEdges, circles, cv::HOUGH_GRADIENT, 1, 20, 70, 20, 10, 0);
    if(circles.empty())
    {
        return BoardPiece::Empty;
    }

    int x = std::round(circles[0][0]);
    int y = std::round(circles[0][1]);
    int r = std::round(circles[0][2]);
    int d = (int)(r / 1.6);

    int divi = 0;
    double sum = 0.0;

    cv::Range rowRange((x - d) < 0 ? 0 : x - d, (x + d) > (dst.cols - 1) ? dst.cols - 1 : x + d);
    cv::Range colsRange((y - d) < 0 ? 0 : y - d, (y + d) > (dst.rows - 1) ? dst.rows - 1 : y + d);
    cv::Mat dstSubmatrix(dst, rowRange, colsRange);
    for(int i = 0; i < dstSubmatrix.rows; i++)
    {
        cv::Mat row = dstSubmatrix.row(i);
        for(int j = 0; j < row.cols; j++)
        {
            sum += row.at<unsigned char>(0, j);
            divi++;
        }
    }

    sum /= divi;
    sum *= 3;

    if(sum > colorThreshold)
    {
        cv::circle(cell, cv::Point(x, y), r, cv::Scalar(0x00, 0xFF, 0x00, 0xFF), 2);
        return BoardPiece::PlayerPiece;
    }
    else
    {
        cv::circle(cell, cv::Point(x, y), r, cv::Scalar(0x00, 0x00, 0xFF, 0xFF), 2);
        return BoardPiece::OpPlayerPiece;
    }
}

int CameraAnalyzer::getColorThreshold() const
{
    return colorThreshold;
}

void CameraAnalyzer::setColorThreshold(int value)
{
    colorThreshold = value;
}

int CameraAnalyzer::getCannyHysterisisUThold() const
{
    return CannyHysterisisUThold;
}

QImage CameraAnalyzer::getLastRawFromCamera()
{
    return lastRawFromCamera;
}

QImage CameraAnalyzer::getLastPieceOverlay()
{
    return lastPieceOverlay;
}

QImage CameraAnalyzer::getLastEdgeView()
{
    return lastEdgeView;
}

bool CameraAnalyzer::startCapture()
{
    if(!errorState && !timerId)
    {
        timerId = startTimer(1000 / framerate);
        return true;
    }
    return false;
}

bool CameraAnalyzer::stopCapture()
{
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
        return true;
    }
    return false;
}

void CameraAnalyzer::captureSingleRawBoard()
{
    boardCaptureRequest = true;
    if(!errorState && !timerId)
    {
        camera->captureImage();
    }
}

void CameraAnalyzer::setCannyHysterisisUThold(int value)
{
    CannyHysterisisUThold = value;
}

void CameraAnalyzer::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    camera->captureImage();
}

int CameraAnalyzer::getCannyHysterisisLThold() const
{
    return CannyHysterisisLThold;
}

void CameraAnalyzer::setCannyHysterisisLThold(int value)
{
    CannyHysterisisLThold = value;
}

//HELPER FN ---------------------------------------------------------------------------------------
namespace
{

inline QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format)
{
    return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), format);
}

inline cv::Mat qimage_to_mat_ref(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(), format, img.bits(), img.bytesPerLine());
}

}
//HELPER FN END -----------------------------------------------------------------------------------

QImage CameraAnalyzer::Mat2QImage_Ref(cv::Mat &mat, bool swap)
{
    if(!mat.empty())
    {
        switch(mat.type())
        {
        case CV_8UC3 :
            if(swap)
            {
                return mat_to_qimage_ref(mat, QImage::Format_RGB888).rgbSwapped();
            }
            else
            {
                return mat_to_qimage_ref(mat, QImage::Format_RGB888);
            }

        case CV_8U :
            return mat_to_qimage_ref(mat, QImage::Format_Indexed8);

        case CV_8UC4 :
            return mat_to_qimage_ref(mat, QImage::Format_ARGB32);
        }
    }

    return QImage();
}

QImage CameraAnalyzer::Mat2QImage(const cv::Mat &mat, bool swap)
{
    return Mat2QImage_Ref(const_cast<cv::Mat&>(mat), swap).copy();
}

cv::Mat CameraAnalyzer::QImage2Mat_Ref(QImage &img, bool swap)
{
    if(!img.isNull())
    {
        switch(img.format())
        {
        case QImage::Format_RGB888:
        {
            auto result = qimage_to_mat_ref(img, CV_8UC3);
            if(swap)
            {
                cv::cvtColor(result, result, CV_RGB2BGR);
            }
            return result;
        }

        case QImage::Format_Indexed8:
            return qimage_to_mat_ref(img, CV_8U);

        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
            return qimage_to_mat_ref(img, CV_8UC4);
        case QImage::Format_RGBA8888:
        {
            auto result = qimage_to_mat_ref(img, CV_8UC4);
            if(swap)
            {
                cv::cvtColor(result, result, CV_RGB2BGR);
            }
            return result;
        }

        default:
            break;
        }
    }

    return cv::Mat();
}

cv::Mat CameraAnalyzer::QImage2Mat(const QImage &img, bool swap)
{
    return QImage2Mat_Ref(const_cast<QImage&>(img), swap).clone();
}

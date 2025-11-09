#include "../inc/ConFaceCheck.h"
#include "../inc/ImageStreamObserver.h"
#include "Login/inc/authService.h"

ConFaceCheckIpml::ConFaceCheckIpml() {
    this->id = "confacecheck";
}

ConFaceCheckIpml::~ConFaceCheckIpml() {}

void ConFaceCheckIpml::processElement(cv::Mat &element) {

    cv::Mat eCopy = element.clone();
    if (FaceCheckService::getInstance()->loginWithFace(element) &&
        !AuthService::getInstance()->isLogined()) {
        Q_EMIT VideoStreamer::getInstance().loginSuccess();
        AuthService::getInstance()->setLogined(true);
    }
    QImage img = QImage(eCopy.data, eCopy.cols, eCopy.rows, QImage::Format_RGB888).rgbSwapped();
    Q_EMIT VideoStreamer::getInstance().newImage(img, this->id);
}
#include "../inc/ProCamIpml.h"
#include "Logger/inc/logger.h"

ProducerCamIpml::ProducerCamIpml() {}

ProducerCamIpml::~ProducerCamIpml() {}

cv::Mat ProducerCamIpml::produceElement() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "ProducerCamIpml: Failed to capture frame. Returning an empty frame."
                  << std::endl;
    }
    return frame;
}

void ProducerCamIpml::init(int cameraIndex) {
    LOG(LogLevel::INFO,
        "ProducerCamIpml: Initializing camera with index " + std::to_string(cameraIndex));
    if (!initFlag) {
        cap = cv::VideoCapture(cameraIndex);
        if (!cap.isOpened()) {
            throw std::runtime_error("Unable to open the camera.");
        }
    }
    this->initFlag = true;
}

void ProducerCamIpml::release(int cameraIndex) {
    this->cap.release();
    LOG(LogLevel::INFO,
        "ProducerCamIpml: Releasing camera with index " + std::to_string(cameraIndex));
    this->initFlag = false;
}

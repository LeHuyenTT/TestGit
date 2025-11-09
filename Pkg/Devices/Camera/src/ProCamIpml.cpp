#include "../inc/ProCamIpml.h"
#include "Logger/inc/logger.h"

ProducerCamIpml::ProducerCamIpml() {}
ProducerCamIpml::~ProducerCamIpml() {}

void ProducerCamIpml::init(int cameraIndex) {
    LOG(LogLevel::INFO,
        "ProducerCamIpml: Initializing camera with index " + std::to_string(cameraIndex));
    if (!initFlag) {
        cap.open(cameraIndex);
        if (!cap.isOpened())
            throw std::runtime_error("Unable to open the camera.");
        isVideoFile = false;
        initFlag = true;
    }
}

void ProducerCamIpml::init(const std::string &videoPath) {
    LOG(LogLevel::INFO, "ProducerCamIpml: Opening video file " + videoPath);
    if (!initFlag) {
        cap.open(videoPath);
        if (!cap.isOpened())
            throw std::runtime_error("Unable to open video file: " + videoPath);
        this->videoPath = videoPath;
        isVideoFile = true;
        initFlag = true;
    }
}

cv::Mat ProducerCamIpml::produceElement() {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        // Nếu là video, cho chạy lại từ đầu
        if (isVideoFile) {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
            cap >> frame;
        }
        if (frame.empty())
            LOG(LogLevel::WARNING, "ProducerCamIpml: No frame captured.");
    }
    return frame;
}

void ProducerCamIpml::release(int cameraIndex) {
    cap.release();
    LOG(LogLevel::INFO,
        isVideoFile
            ? "ProducerCamIpml: Releasing video file " + videoPath
            : "ProducerCamIpml: Releasing camera " + std::to_string(cameraIndex));
    initFlag = false;
}

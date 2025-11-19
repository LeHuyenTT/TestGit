#include "../inc/CamThreadMgr.h"
#include "Logger/inc/logger.h"
#include <algorithm>

CamThreadMgr *CamThreadMgr::instance = nullptr;
std::mutex    CamThreadMgr::mutex;

CamThreadMgr::CamThreadMgr(QObject *parent) : QObject(parent) {
    this->safeQueue = std::make_shared<SafeQueue<cv::Mat>>(MAX_QUEUE_SIZE);
    producer        = std::make_unique<ProducerCamIpml>();
    producer->setQueue(this->safeQueue);
    producer->setSleepTime(0);
    producer->setProducerName("CameraProducer");
    producer->setProducerId(1);
}

CamThreadMgr *CamThreadMgr::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new CamThreadMgr();
    }
    return instance;
}

CamThreadMgr::~CamThreadMgr() {}

void CamThreadMgr::setCameraIndex(int cameraIndex) {
    this->cameraIndex = cameraIndex;
    if (producer)
        producer->init(this->cameraIndex);
}

void CamThreadMgr::setVideoPath(const std::string &path) {
    this->videoPath = path;
    if (producer)
        producer->init(this->videoPath);
}

void CamThreadMgr::addConsumer(std::unique_ptr<Consumer<cv::Mat>> consumer) {
    consumer->setQueue(this->safeQueue);
    consumers.push_back(std::move(consumer));
}

void CamThreadMgr::deleteConsumerByID(uint8_t consumerId) {
    consumers.erase(std::remove_if(consumers.begin(), consumers.end(),
                                   [consumerId](const auto &c) {
                                       if (c->getConsumerId() == consumerId) {
                                           c->stop();
                                           return true;
                                       }
                                       return false;
                                   }),
                    consumers.end());
}

void CamThreadMgr::stopProducer() {
    if (producer) {
        producer->stop();
        producer->release(this->cameraIndex);
    }
    this->safeQueue.reset();
}

void CamThreadMgr::startProducer() {
    this->safeQueue = std::make_shared<SafeQueue<cv::Mat>>(MAX_QUEUE_SIZE);
    if (producer) {
        producer->run();
    } else {
        LOG(LogLevel::ERROR, "Producer not initialized (camera or video missing)");
    }
}

void CamThreadMgr::startAllConsumers() {
    for (auto &c : consumers)
        c->run();
}

void CamThreadMgr::stopAllConsumers() {
    for (auto &c : consumers)
        c->stop();
}

bool CamThreadMgr::startConsumerByID(int consumerId) {
    for (auto &c : consumers) {
        if (c->getConsumerId() == consumerId) {
            c->run();
            return true;
        }
    }
    return false;
}

bool CamThreadMgr::stopConsumerByID(int consumerId) {
    for (auto &c : consumers) {
        if (c->getConsumerId() == consumerId) {
            c->stop();
            return true;
        }
    }
    return false;
}

bool CamThreadMgr::producerIsRunning() {
    return producer && producer->isRunningState();
}

int CamThreadMgr::getNumConsumers() {
    return consumers.size();
}

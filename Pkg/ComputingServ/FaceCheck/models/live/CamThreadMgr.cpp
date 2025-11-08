#include "../inc/CamThreadMgr.h"
#include "Logger/inc/logger.h"
#include <algorithm>

CamThreadMgr *CamThreadMgr::instance = nullptr;
std::mutex    CamThreadMgr::mutex;

CamThreadMgr::CamThreadMgr(QObject *parent) : QObject(parent) {
#ifndef BUILD_VIRTUAL
    cameraIndex     = 0;
    this->safeQueue = std::make_shared<SafeQueue<cv::Mat>>(MAX_QUEUE_SIZE);
    producer        = std::make_unique<ProducerCamIpml>();
    producer->init(this->cameraIndex);
    producer->setQueue(this->safeQueue);
    producer->setSleepTime(0);
    producer->setProducerName("CameraProducer");
    producer->setProducerId(1);
#endif
}

CamThreadMgr *CamThreadMgr::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new CamThreadMgr();
    }
    return instance;
}

CamThreadMgr::~CamThreadMgr(){};

void CamThreadMgr::setCameraIndex(int cameraIndex) {
    this->cameraIndex = cameraIndex;
    producer->init(this->cameraIndex);
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
#ifndef BUILD_VIRTUAL
    if (producer) {
        producer->stop();
    }
    this->producer->release(this->cameraIndex);
    // delete safeQueue;
    this->safeQueue.reset();
#endif
}

void CamThreadMgr::startProducer() {
    this->safeQueue = std::make_shared<SafeQueue<cv::Mat>>(MAX_QUEUE_SIZE);
    if (producer) {
        producer->run();
    }
}

void CamThreadMgr::startAllConsumers() {
    for (auto &c : consumers) {
        c->run();
    }
}

void CamThreadMgr::stopAllConsumers() {
    for (auto &c : consumers) {
        c->stop();
    }
}

bool CamThreadMgr::startConsumerByID(int consumerId) {
#ifndef BUILD_VIRTUAL
    for (auto &c : consumers) {
        if (c->getConsumerId() == consumerId) {
            c->run();
            return true;
        }
    }
#endif
    return false;
}

bool CamThreadMgr::stopConsumerByID(int consumerId) {
#ifndef BUILD_VIRTUAL
    for (auto &c : consumers) {
        if (c->getConsumerId() == consumerId) {
            c->stop();
            return true;
        }
    }
#endif
    return false;
}

bool CamThreadMgr::producerIsRunning() {
    return producer && producer->isRunningState();
}

int CamThreadMgr::getNumConsumers() {
    return consumers.size();
}
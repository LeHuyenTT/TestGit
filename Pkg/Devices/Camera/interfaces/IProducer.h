#ifndef PRODUCER_H
#define PRODUCER_H

#include "SafeQueue.h"
#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

template <typename T> class Producer {
public:
    Producer() : sleepTime(0), producerName(""), producerId(0), isRunning(false) {}

    Producer(std::shared_ptr<SafeQueue<T>> queue, uint16_t sleepTime,
             const std::string &producerName, uint8_t producerId)
        : safeQueue(queue), sleepTime(sleepTime), producerName(producerName),
          producerId(producerId), isRunning(false) {}

    ~Producer() = default;

    virtual void run() {
        isRunning      = true;
        producerThread = std::thread([this] {
            while (isRunning) {
                T ele = produceElement();
                safeQueue->push(std::move(ele));
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        });
    }

    virtual void stop() {
        isRunning = false;
        if (producerThread.joinable()) {
            producerThread.join();
        }
    }

    virtual void wait() {
        if (producerThread.joinable()) {
            producerThread.join();
        }
    }

    virtual void setQueue(std::shared_ptr<SafeQueue<T>> queue) {
        safeQueue = queue;
    }

    virtual void setSleepTime(uint16_t sleepTime) {
        this->sleepTime = sleepTime;
    }

    virtual void setProducerName(const std::string &producerName) {
        this->producerName = producerName;
    }

    virtual void setProducerId(uint8_t producerId) {
        this->producerId = producerId;
    }

    virtual bool isRunningState() {
        return isRunning;
    }

    virtual T produceElement() = 0;

private:
    std::shared_ptr<SafeQueue<T>> safeQueue;
    uint16_t                      sleepTime;
    std::string                   producerName;
    uint8_t                       producerId;
    std::atomic<bool>             isRunning;
    std::thread                   producerThread;
};

#endif // PRODUCER_H

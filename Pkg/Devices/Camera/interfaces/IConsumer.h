#ifndef CONSUMER_H
#define CONSUMER_H

#include "SafeQueue.h"
#include <atomic>
#include <memory>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

template <typename T> class Consumer {
public:
    Consumer() : sleepTime(0), consumerName(""), consumerId(0), isRunning(false) {}

    Consumer(std::shared_ptr<SafeQueue<T>> queue, uint16_t sleepTime,
             const std::string &consumerName, uint8_t consumerId)
        : safeQueue(queue), sleepTime(sleepTime), consumerName(consumerName),
          consumerId(consumerId), isRunning(false) {}

    ~Consumer() = default;

    virtual void run() {
        isRunning      = true;
        consumerThread = std::thread([this] {
            while (isRunning) {
                auto element = safeQueue->pop();
                processElement(element);
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        });
    }

    virtual void stop() {
        isRunning = false;
        if (consumerThread.joinable()) {
            consumerThread.join();
            // safeQueue->empty();
        }
    }

    virtual void wait() {
        if (consumerThread.joinable()) {
            consumerThread.join();
        }
    }

    virtual void setQueue(std::shared_ptr<SafeQueue<T>> queue) {
        safeQueue = queue;
    }

    virtual SafeQueue<T> *getQueue() {
        return safeQueue.get();
    }

    virtual void setSleepTime(uint16_t sleepTime) {
        this->sleepTime = sleepTime;
    }

    virtual void setConsumerName(const std::string &consumerName) {
        this->consumerName = consumerName;
    }

    virtual void setConsumerId(uint8_t consumerId) {
        this->consumerId = consumerId;
    }

    virtual uint8_t getConsumerId() const {
        return this->consumerId;
    }

    virtual void processElement(T &element) = 0;

    virtual std::string getConsumerName() const {
        return this->consumerName;
    }

private:
    std::shared_ptr<SafeQueue<T>> safeQueue;
    uint16_t                      sleepTime;
    std::string                   consumerName;
    uint8_t                       consumerId;
    std::atomic<bool>             isRunning;
    std::thread                   consumerThread;
};

#endif // CONSUMER_H

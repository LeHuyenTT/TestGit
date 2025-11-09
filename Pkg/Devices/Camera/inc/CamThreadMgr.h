#ifndef CAMTHREADMGR_H
#define CAMTHREADMGR_H

#include "../interfaces/IConsumer.h"
#include "../interfaces/IProducer.h"
#include "../interfaces/SafeQueue.h"
#include "ProCamIpml.h"
#include <QObject>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <vector>

#define MAX_QUEUE_SIZE 10

class CamThreadMgr : public QObject {
    Q_OBJECT
private:
    static CamThreadMgr *instance;
    static std::mutex    mutex;

    int         cameraIndex = 0;
    std::string videoPath;

    CamThreadMgr(QObject *parent = nullptr);
    CamThreadMgr(const CamThreadMgr &) = delete;
    CamThreadMgr &operator=(const CamThreadMgr &) = delete;

    std::unique_ptr<ProducerCamIpml>                producer;
    std::vector<std::unique_ptr<Consumer<cv::Mat>>> consumers;
    std::shared_ptr<SafeQueue<cv::Mat>>             safeQueue;

public:
    static CamThreadMgr *getInstance();
    virtual ~CamThreadMgr();

    // --- Producer control ---
    Q_INVOKABLE void startProducer();
    Q_INVOKABLE void stopProducer();

    // --- Consumer control ---
    void             addConsumer(std::unique_ptr<Consumer<cv::Mat>> consumer);
    void             deleteConsumerByID(uint8_t consumerId);
    void             startAllConsumers();
    void             stopAllConsumers();
    Q_INVOKABLE bool startConsumerByID(int consumerId);
    Q_INVOKABLE bool stopConsumerByID(int consumerId);

    // --- Configuration ---
    Q_INVOKABLE void setCameraIndex(int cameraIndex);
    void             setVideoPath(const std::string &path);

    // --- Info ---
    bool producerIsRunning();
    int  getNumConsumers();
};

#endif // CAMTHREADMGR_H

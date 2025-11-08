#ifndef PROCAMIPML_H
#define PROCAMIPML_H
#include "../interfaces/IProducer.h"
#include <opencv2/opencv.hpp>

class ProducerCamIpml : public Producer<cv::Mat> {
private:
    cv::VideoCapture cap;

public:
    ProducerCamIpml();
    ~ProducerCamIpml();
    bool    initFlag = false;
    void    init(int cameraIndex);
    void    release(int cameraIndex);
    cv::Mat produceElement() override;
};

#endif // PROCAMIPML_H
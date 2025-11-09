#ifndef CONCAMIPML_H
#define CONCAMIPML_H

#include "../interfaces/IConsumer.h"

class ConsumerCamIpml : public Consumer<cv::Mat> {
public:
    ConsumerCamIpml();
    ~ConsumerCamIpml();

    void processElement(cv::Mat &element) override;
};

#endif // CONCAMIPML_H
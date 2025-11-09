#include "../inc/ConCamIpml.h"

ConsumerCamIpml::ConsumerCamIpml() {}

ConsumerCamIpml::~ConsumerCamIpml() {}

void ConsumerCamIpml::processElement(cv::Mat &element) {
    cv::imshow(this->getConsumerName(), element);
    cv::waitKey(1);
}

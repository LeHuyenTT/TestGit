#ifndef TARCFACE_H
#define TARCFACE_H

#include <cmath>
#include <ncnn/net.h>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

using namespace std;

class TArcFace {
private:
    ncnn::Net net;
    const int feature_dim = 128;
    cv::Mat   Zscore(const cv::Mat &fc);

public:
    TArcFace(void);
    ~TArcFace(void);

    cv::Mat GetFeature(cv::Mat img);
};

#endif

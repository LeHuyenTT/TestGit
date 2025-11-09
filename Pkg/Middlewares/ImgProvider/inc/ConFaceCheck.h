#ifndef CONFACECHECK_H
#define CONFACECHECK_H

#include <QImage>
#include <QObject>
#include <QString>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#include "Camera/interfaces/IConsumer.h"
#include "FaceCheck/inc/FaceCheckService.h"

class ConFaceCheckIpml : public Consumer<cv::Mat> {
public:
    ConFaceCheckIpml();
    ~ConFaceCheckIpml();
    bool isCheckOK = false;

    void processElement(cv::Mat &element) override;

private:
    QString id;
};

#endif // CONFACECHECK_H

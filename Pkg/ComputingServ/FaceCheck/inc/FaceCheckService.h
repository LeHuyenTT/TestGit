#ifndef FACECHECKSERVICE_H
#define FACECHECKSERVICE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TArcface.h"
#include "TBlur.h"
#include "TLive.h"
#include "TMtCNN.h"
#include "TRetina.h"
#include "TWarp.h"

#include <QObject>
#include <algorithm>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

using namespace cv;

//#define RETINA                                    // comment if you want to use MtCNN landmark detection instead
#define TEST_LIVING
#define MIN_FACE_THRESHOLD 0.50
#define FACE_LIVING 0.93

class FaceCheckService : public QObject {
    Q_OBJECT
private:
    static FaceCheckService *instance;
    static std::mutex        m_ctx;

    FaceCheckService(const FaceCheckService &) = delete;
    FaceCheckService &operator=(const FaceCheckService &) = delete;

    FaceCheckService(QObject *parent = nullptr);
    ~FaceCheckService();

    const int RetinaWidth  = 320;
    const int RetinaHeight = 240;

    //! @brief Neural network modules
    TLive                   Live;
    TWarp                   Warp;
    TMtCNN                  MtCNN;
    TArcFace                ArcFace;
    TRetina *               Rtn;

    std::vector<cv::String> NameFaces;
    std::vector<cv::Mat>    fc1;
    int                     faceCnt = 0;

    // ✅ Đường dẫn DB của bạn
    std::string path_to_dir = "/workspaces/TestGit/DB";
    // ✅ Ảnh bạn tải về là PNG
    std::string endswith    = ".jpg";
    std::string pattern_jpg = "";

public:
    static FaceCheckService *getInstance();

    bool             init(std::string id);
    bool             getResources(const std::string &id);
    bool             loadModelCustom();
    bool             loginWithFace(const cv::Mat &img);
    Q_INVOKABLE bool recognize(QString id, int cam_num = 0);
    bool             loadDatabase();

    std::string setPatternStr(std::string id);
    cv::Mat     getFeatureFromImage(const cv::Mat &img);
};

#endif // FACECHECKSERVICE_H

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Camera/inc/CamThreadMgr.h"
#include "FaceCheck/inc/FaceCheckService.h"
#include "FaceMesh/inc/FaceMeshService.h"
#include "ImgProvider/inc/ConFaceCheck.h"
#include "ImgProvider/inc/ImageProvider.h"
#include "Common/constant.h"
#include "Booting/inc/BootingApp.h"
#include "Logger/inc/logger.h"

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QCoreApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // --- Bước 1: Booting app (tải danh sách ảnh từ API và lưu về ./DB hoặc /tmp/DB)
    BootApp::getInstance().init();

    // --- Bước 2: Khởi tạo dịch vụ FaceCheckService với ID (ví dụ "20521422")
    FaceCheckService::getInstance()->init("20521422");
    // → Hàm init() sẽ gọi setPatternStr() và loadDatabase(), không còn lỗi pattern_jpg
    // cv::Mat img = cv::imread("/tmp/DB/20521422_1.jpg");
    // cv::Mat feat = FaceCheckService::getInstance()->getFeatureFromImage(img);
    // std::cout << "feature shape: " << feat.rows << "x" << feat.cols << std::endl;
    // --- Bước 3: Khởi tạo video test thay cho camera thật
    CamThreadMgr::getInstance()->setVideoPath("./test/Huyen.mp4");

    std::unique_ptr<Consumer<cv::Mat>> conFaceCheck = std::make_unique<ConFaceCheckIpml>();
    conFaceCheck->setSleepTime(0);
    conFaceCheck->setConsumerName("conFaceCheck");
    conFaceCheck->setConsumerId(1);

    CamThreadMgr::getInstance()->addConsumer(std::move(conFaceCheck));
    CamThreadMgr::getInstance()->startProducer();

    // --- Bước 4: Nạp các mô hình xử lý mesh
    FaceMeshService::getInstance()->load("500m");

    // --- Bước 5: Chạy event loop Qt
    bool match = FaceCheckService::getInstance()->recognize("20521422", 0);

    if (match)
        LOG(LogLevel::INFO, "✅ Người trong video trùng với ảnh trong DB!");
    else
        LOG(LogLevel::WARNING, "❌ Người trong video KHÔNG trùng với ảnh trong DB!");

    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Face recognition related headers
#include "Camera/inc/CamThreadMgr.h"
#include "FaceCheck/inc/FaceCheckService.h"
#include "FaceMesh/inc/FaceMeshService.h"
#include "ImgProvider/inc/ConFaceCheck.h"
#include "ImgProvider/inc/ImageProvider.h"
#include "Common/constant.h"

// STD Library
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Image provider for live camera feed
    ImageProvider *liveImageProvider = new ImageProvider;

    // --- Initialize camera ---
    CamThreadMgr::getInstance()->setCameraIndex(CAMERA_INDEX);

    // Consumer for face checking
    std::unique_ptr<Consumer<cv::Mat>> conFaceCheck = std::make_unique<ConFaceCheckIpml>();
    conFaceCheck->setSleepTime(0);
    conFaceCheck->setConsumerName("conFaceCheck");
    conFaceCheck->setConsumerId(1);

    CamThreadMgr::getInstance()->addConsumer(std::move(conFaceCheck));
    CamThreadMgr::getInstance()->startProducer();

    // --- Bind C++ objects to QML ---
    engine.rootContext()->setContextProperty("CamThreadService", CamThreadMgr::getInstance());
    engine.rootContext()->setContextProperty("liveImageProvider", liveImageProvider);
    engine.rootContext()->setContextProperty("FaceCheckService", FaceCheckService::getInstance());

    engine.addImageProvider("live", liveImageProvider);

    // --- Initialize face recognition modules ---
    FaceMeshService::getInstance()->load("500m");
    FaceCheckService::getInstance()->loadDatabase();

    // --- Load UI ---
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

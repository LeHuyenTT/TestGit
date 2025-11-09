#include "Auth/Login/inc/authService.h"
#include "Booting/inc/BootingApp.h"
#include "Common/constant.h"
#include <thread>

std::mutex   AuthService::m_ctx;
AuthService *AuthService::m_instance = nullptr;

AuthService::AuthService(QObject *parent) : QObject(parent) {
    m_isLogined = false;
}

bool AuthService::loginToServer(QVariant v_jsonObj) {
    if (v_jsonObj.isNull()) {
        std::cout << "AuthService::loginToServer: v_jsonObj is null" << std::endl;
        return false;
    }

    QJsonObject jsonObject = QJsonObject::fromVariantMap(v_jsonObj.toMap());

    bool ret = true;
    HttpClientImpl::getInstance()->sendPostRequest(
        QUrl(URL_USER_LOGIN), jsonObject, [&](QByteArray response) {
            QJsonObject res_json = QJsonDocument::fromJson(response).object();
            if (res_json.value("success").toBool()) {
                if (res_json["data"].toObject().value("status").toString().compare("online") == 0) {
                    ret = false;
                    std::cout << "AuthService::loginToServer: user is online" << std::endl;
                } else {
                    //User::getInstance()->userFromJson(res_json["data"].toObject());
                    ret         = true;
                    m_isLogined = true;
                }
            }
        });

    return ret;
}

AuthService *AuthService::getInstance() {
    m_ctx.lock();
    if (m_instance == nullptr) {
        m_instance = new AuthService();
    }
    m_ctx.unlock();
    return m_instance;
}

QString AuthService::getIdTeacher() {
    //return BootApp::getInstance().getIDTeacherCur();
}

AuthService::~AuthService() {
    if (this->m_instance != nullptr)
        delete m_instance;
}

void AuthService::logOut() {
   // SocketIoClient::getInstance().dSyncClose();
    m_isLogined = false;
}

bool AuthService::isLogined() {
    return m_isLogined;
}

void AuthService::setLogined(bool isLogined) {
    m_isLogined = isLogined;
}
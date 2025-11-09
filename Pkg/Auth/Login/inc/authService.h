#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QObject>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <mutex>
#include <iostream>

#include "Booting/inc/BootingApp.h"
#include "Common/constant.h"
#include "Http/inc/httpclientimpl.h"
//#include "MVC/Controller/inc/user.h"
#include "Session/inc/SessionService.h"

using std::placeholders::_1;

class AuthService : public QObject {
    Q_OBJECT

private:
    explicit AuthService(QObject *parent = nullptr);
    ~AuthService();

    AuthService(const AuthService &) = delete;
    AuthService &operator=(const AuthService &) = delete;

    bool                m_isLogined;
    QString             m_token;
    static AuthService *m_instance;
    static std::mutex   m_ctx;

public:
    static AuthService *getInstance();

    // Controller
    Q_INVOKABLE bool    loginToServer(QVariant vj);
    Q_INVOKABLE QString getIdTeacher();
    Q_INVOKABLE void    logOut();
    void                setLogined(bool isLogined);
    bool                isLogined();
};

#endif // AUTHSERVICE_H

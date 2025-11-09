#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H
#include <QObject>
#include <QString>
#include <iostream>
#include <mutex>

class SessionService : public QObject {
    Q_OBJECT
    SessionService(QObject *parent = nullptr);
    ~SessionService();

    SessionService(SessionService const &) = delete;
    void operator=(SessionService const &) = delete;

    static SessionService *m_instance;
    static std::mutex      m_ctx;

    /* data */
    QString m_sessionId;

public:
    static SessionService *getInstance();
    Q_INVOKABLE QString    getSessionKey();
    Q_INVOKABLE void       setSessionKey(QString key);
};

#endif // SESSIONSERVICE_H

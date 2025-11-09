#include "../inc/SessionService.h"

std::mutex      SessionService::m_ctx;
SessionService *SessionService::m_instance = nullptr;

SessionService::SessionService(QObject *parent) : QObject(parent) {
    m_sessionId = "";
}

SessionService::~SessionService() {
    //    if (!m_instance){
    //        delete m_instance;
    //    }
}

SessionService *SessionService::getInstance() {
    m_ctx.lock();
    if (!m_instance) {
        m_instance = new SessionService();
    }
    m_ctx.unlock();
    return m_instance;
}

QString SessionService::getSessionKey() {
    return this->m_sessionId;
}

void SessionService::setSessionKey(QString key) {
    this->m_sessionId = key;
}

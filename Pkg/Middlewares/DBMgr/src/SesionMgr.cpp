#include "../inc/SessionMgr.h"

void SessionMgr::initDatabase() {
    if (!m_dbMgr.createTable(tableName)) {
        std::cerr << "Failed to create table " << tableName.toStdString() << std::endl;
    }
}

bool SessionMgr::addSession(const QString &userId, const QString &session) {
    bool      success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO sessions (userid, session) VALUES (:userid, :session)");
    query.bindValue(":userid", userId);
    query.bindValue(":session", session);
    if (query.exec()) {
        success = true;
    } else {
        qDebug() << "addPerson error:" << query.lastError();
    }
    return success;
}

bool SessionMgr::deleteSession(const QString &userId) {
    bool      ret = false;
    QSqlQuery query;
    query.prepare("DELETE FROM sessions WHERE userid = :userid");
    query.bindValue(":userid", userId);
    if (query.exec()) {
        ret = true;
    } else {
        qDebug() << "addPerson error:" << query.lastError();
    }
    return ret;
}

bool SessionMgr::getSession(const QString &userId) {
    bool      ret = false;
    QSqlQuery query;
    query.prepare("SELECT session FROM sessions WHERE userid = :userid");
    query.bindValue(":userid", userId);
    if (query.exec()) {
        if (query.next()) {
            QString session = query.value(0).toString();
            qDebug() << "Session found for user" << userId << ":" << session;
            ret = true;
        } else {
            qDebug() << "No session found for user" << userId;
        }
    } else {
        qDebug() << "getSession error:" << query.lastError();
    }
    return ret;
}

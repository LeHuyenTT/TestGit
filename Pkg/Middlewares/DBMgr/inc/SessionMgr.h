#ifndef SESSIONMGR_H
#define SESSIONMGR_H
#include "DBMgr.h"
#include <QObject>
#include <QVariant>

class SessionMgr : public QObject {
    Q_OBJECT
public:
    SessionMgr(const QString &dbPath, const QString &tableName)
        : m_dbMgr(DBMgr::getInstance(dbPath)), tableName("sessions") {
        initDatabase();
    }
    virtual void initDatabase();
    virtual bool deleteSession(const QString &userId);
    virtual bool getSession(const QString &userId);
    virtual bool addSession(const QString &userId, const QString &session);

private:
    DBMgr & m_dbMgr;
    QString tableName;
};

#endif // SESSIONMGR_H

#include "../inc/DBMgr.h"
#include "Logger/inc/logger.h"

// Initialize static members
DBMgr *    DBMgr::instance = nullptr;
std::mutex DBMgr::mtx;

DBMgr::DBMgr(const QString &path) : QObject(nullptr), db(QSqlDatabase::addDatabase("QSQLITE")) {
    db.setDatabaseName(path);
    if (!db.open()) {
        LOG(LogLevel::ERROR,
            "Failed to connect to database:" + db.lastError().text().toStdString());
    }
}

bool DBMgr::createTable(const QString &tableName, QString cmdCreateTable) {
    QSqlQuery query(db);
    QString   queryString =
        QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(tableName);
    if (!query.exec(queryString)) {
        LOG(LogLevel::ERROR,
            "Failed to check if table exists:" + query.lastError().text().toStdString());
        return false;
    }
    if (query.next()) {
        LOG(LogLevel::INFO, "Table already exists:" + tableName.toStdString());
        return true;
    }
    if (cmdCreateTable.isEmpty()) {
        cmdCreateTable = QString("CREATE TABLE %1 (userid VARCHAR(255) PRIMARY "
                                 "KEY, session VARCHAR(255))")
                             .arg(tableName);
    }
    if (!query.exec(cmdCreateTable)) {
        qCritical() << "Failed to create table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DBMgr::deleteTable(const QString &tableName) {
    QSqlQuery query(db);
    QString   queryString = QString("DROP TABLE IF EXISTS %1").arg(tableName);
    if (!query.exec(queryString)) {
        qCritical() << "Failed to delete table:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery DBMgr::query(const QString &tableName, const QString &queryCmd) {
    QString   queryString = queryCmd.arg(tableName);
    QSqlQuery query(db);
    if (!query.exec(queryString)) {

        qCritical() << "Failed to execute query:" << query.lastError().text();
    }
    return query;
}

DBMgr::~DBMgr() {
    db.close();
}
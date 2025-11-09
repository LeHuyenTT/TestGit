#ifndef DB_MGR_H
#define DB_MGR_H

#include <QObject>
#include <QtSql>
#include <iostream>
#include <mutex>

class DBMgr : public QObject {
    Q_OBJECT
private:
    DBMgr(const QString &path);                          // Constructor with path argument
    ~DBMgr();                                            // Private destructor
    DBMgr(const DBMgr &)      = delete;                  // Private copy constructor
    DBMgr &           operator=(const DBMgr &) = delete; // Private copy assignment operator
    static std::mutex mtx;                               // Mutex for thread-safe access
    static DBMgr *    instance;                          // Singleton instance pointer
    QSqlDatabase      db;                                // Qt SQLite database object

public:
    static DBMgr &getInstance(const QString &path) {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) {
                instance = new DBMgr(path);
            }
        }
        return *instance;
    }

    virtual bool      createTable(const QString &tableName, QString queryCmdCreateTable = "");
    virtual bool      deleteTable(const QString &tableName);
    virtual QSqlQuery query(const QString &tableName, const QString &queryCmd = "SELECT * FROM %1");
};

#endif // DB_MGR_H
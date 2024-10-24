#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class Database {
public:
    static Database& instance() {
        static Database instance;
        return instance;
    }

    bool connectToDatabase(const QString& host, const QString& dbName, const QString& user, const QString& password);
    bool isValidUser(const QString& username, const QString& password);
    bool registerUser(const QString& username, const QString& password);
    bool initializeDatabase();

private:
    Database();
    ~Database();
    QSqlDatabase db;
};

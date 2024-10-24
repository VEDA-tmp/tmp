#include "database.h"
#include <QSqlError>
#include <QDebug>

Database::Database() {
    // 기본적으로 SQLite를 사용하도록 설정
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Database::connectToDatabase(const QString& host, const QString& dbName, const QString& user, const QString& password) {
    // SQLite는 host, user, password가 필요하지 않음
    Q_UNUSED(host);
    Q_UNUSED(user);
    Q_UNUSED(password);

    db.setDatabaseName(dbName);

    // 데이터베이스 열기
    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
        return false;
    }

    // 데이터베이스가 새로 생성된 경우 테이블을 생성
    if (!initializeDatabase()) {
        qDebug() << "Failed to initialize the database.";
        return false;
    }

    qDebug() << "Database connected and initialized.";
    return true;
}

bool Database::initializeDatabase() {
    QSqlQuery query;

    // users 테이블이 없을 경우 생성
    if (!query.exec("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT NOT NULL UNIQUE, "
                    "password TEXT NOT NULL)")) {
        qDebug() << "Failed to create users table: " << query.lastError().text();
        return false;
    }

    qDebug() << "Database initialized successfully.";
    return true;
}

bool Database::isValidUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Login query execution failed: " << query.lastError().text();
        return false;
    }

    return query.next();  // 결과가 있으면 true
}

bool Database::registerUser(const QString& username, const QString& password) {
    QSqlQuery query;
    
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Registration query execution failed: " << query.lastError().text();
        return false;
    }

    return true;
}

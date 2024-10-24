#include "auth.h"
#include "utils.h"
#include "database.h"
#include <QJsonDocument>
#include <QJsonObject>


Auth::Auth(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Auth::onNewConnection);
}

void Auth::startServer(quint16 port) {
    if(!server->listen(QHostAddress::Any, port)) {
        qDebug() << "http server failed to start on port " << port;
        LogModel::instance().addLog("Error", get_currentTime(), "server", "http server failed to start on port.");
        
    } else {
        LogModel::instance().addLog("Info", get_currentTime(), "server", "http server start");
        qDebug() << "http server started on port" << port;
    }
}

void Auth::onNewConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Auth::onReadyRead);
}

void Auth::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray request = socket->readAll();
    // 로그인 요청이 있을 때 라우터로 시그널 전송
    emit routeRequest(socket, request);
}

void Auth::sendErrorResponse(QTcpSocket *socket, const QString &message) {
    QJsonObject response;
    response["status"] = "400";
    response["message"] = message;

    QJsonDocument responseDoc(response);
    QByteArray jsonResponse = responseDoc.toJson();
    QByteArray httpResponse = "HTTP/1.1 400 Bad Request\r\nContent-Type: application/json\r\n"
                              "Content-Length: " + QByteArray::number(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
    socket->write(httpResponse);
    socket->flush();
    socket->close();
}

void Auth::handleLogin(QTcpSocket *socket, const QByteArray &request) {
    // JSON 형식으로 로그인 정보를 추출
    QByteArray body = request.mid(request.indexOf("\r\n\r\n") + 4);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    QJsonObject jsonObj = jsonDoc.object();
    QString username = jsonObj["username"].toString();
    QString password = jsonObj["password"].toString();

    QJsonObject response;
    if (Database::instance().isValidUser(username, password)) {
        response["status"] = "200";
        response["token"] = "dummy_token";  // 실제 애플리케이션에서는 JWT 또는 다른 토큰을 생성해야 함
        LogModel::instance().addLog("Info", get_currentTime(), "auth", "login Success");
    } else {
        response["status"] = "400";
        response["message"] = "Invalid username or password";
        LogModel::instance().addLog("Info", get_currentTime(), "auth", "login Failed");
    }

    QByteArray jsonResponse = QJsonDocument(response).toJson();
    QByteArray httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                              "Content-Length: " + QByteArray::number(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
    socket->write(httpResponse);
    socket->flush();
    socket->close();
}

void Auth::handleRegister(QTcpSocket *socket, const QByteArray &request) {
    // JSON 형식으로 회원가입 정보를 추출
    qDebug() << "Request received:" << request;
    QByteArray body = request.mid(request.indexOf("\r\n\r\n") + 4);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
    QJsonObject jsonObj = jsonDoc.object();
    QString username = jsonObj["username"].toString();
    QString password = jsonObj["password"].toString();
    qDebug() << username;
    qDebug() << password;

    QJsonObject response;
    if (Database::instance().registerUser(username, password)) {
        response["status"] = "200";
        LogModel::instance().addLog("Info", get_currentTime(), "auth", "registration Success");
    } else {
        response["status"] = "400";
        response["message"] = "Registration failed";
        LogModel::instance().addLog("Info", get_currentTime(), "auth", "registration Failed");
    }

    QByteArray jsonResponse = QJsonDocument(response).toJson();
    QByteArray httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                              "Content-Length: " + QByteArray::number(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
    socket->write(httpResponse);
    socket->flush();
    socket->close();
}

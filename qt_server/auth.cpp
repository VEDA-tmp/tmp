#include "auth.h"
#include <QJsonDocument>
#include <QJsonObject>


Auth::Auth(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Auth::onNewConnection);
}

void Auth::startServer(quint16 port) {
    if(!server->listen(QHostAddress::Any, port)) {
        qDebug() << "HTTP Server failed to start on port " << port;
    } else {
        qDebug() << "HTTP Server started on port" << port;
    }
}


void Auth::onNewConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Auth::onReadyRead);
}

void Auth::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray request = socket->readAll();
    handleLogin(socket, request);
}

void Auth::handleLogin(QTcpSocket *socket, const QByteArray &request) {
    // 로그인 처리
    QString username = "user1";
    QString password = "password123";

    QJsonObject response;
    if (request.contains(username.toUtf8()) && request.contains(password.toUtf8())) {
        response["status"] = "success";
        response["token"] = "dummy_token";
    } else {
        response["status"] = "error";
    }

    QJsonDocument jsonDoc(response);
    QByteArray jsonResponse = jsonDoc.toJson();
    QByteArray httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n"
                              "Content-Length: " + QByteArray::number(jsonResponse.size()) + "\r\n\r\n" + jsonResponse;
    socket->write(httpResponse);
    socket->flush();
    socket->close();
}
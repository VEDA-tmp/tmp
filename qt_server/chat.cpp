#include "chat.h"

Chat::Chat(QObject *parent) : QObject(parent) {
    server = new QWebSocketServer(QStringLiteral("Chat Server"), QWebSocketServer::NonSecureMode, this);
}

void Chat::startServer(quint16 port) {
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "WebSocket Server failed to start on port" << port;
    } else {
        qDebug() << "WebSocket Server started on port" << port;
        connect(server, &QWebSocketServer::newConnection, this, &Chat::onNewConnection);
    }
}

void Chat::onNewConnection() {
    QWebSocket *socket = server->nextPendingConnection();
    clients << socket;

    connect(socket, &QWebSocket::textMessageReceived, this, &Chat::onTextMessageReceived);
    connect(socket, &QWebSocket::disconnected, this, [this, socket]() {
        clients.removeAll(socket);
        socket->deleteLater();
    });
}

void Chat::onTextMessageReceived(const QString &message) {
    handleMessage(qobject_cast<QWebSocket*>(sender()), message);
}

void Chat::handleMessage(QWebSocket *socket, const QString &message) {
    // 채팅 메시지 처리
    for (QWebSocket *client : std::as_const(clients)) {
        client->sendTextMessage(message);  // 에코 메시지
    }
}

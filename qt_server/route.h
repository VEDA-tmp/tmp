#pragma once
#include <QObject>
#include "auth.h"
#include "chat.h"

class Router : public QObject {
    Q_OBJECT

public:
    Router(QObject *parent = nullptr);
    void start();

private slots:
    void routeHttpRequest(QTcpSocket *socket, const QByteArray &request);

private:
    Auth *authHandler;
    Chat *chatHandler;

    void routeWebSocketRequest(QWebSocket *socket, const QString &message);
};

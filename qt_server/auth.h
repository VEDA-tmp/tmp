#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "logmodel.h"

class Auth : public QObject {
    Q_OBJECT

public:
    Auth(QObject *parnet = nullptr);
    void startServer(quint16 port);
    void handleLogin(QTcpSocket *socket, const QByteArray &request);
    void handleRegister(QTcpSocket *socket, const QByteArray &request);
    void sendErrorResponse(QTcpSocket *socket, const QString &message);

signals:
    void routeRequest(QTcpSocket *socket, const QByteArray &request);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *server;
};
#pragma once
#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>

class Chat : public QObject {
    Q_OBJECT

public:
    Chat(QObject *parent = nullptr);
    void startServer(quint16 port);
    void handleMessage(QWebSocket *socket, const QString &message);

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocketServer *server;
    QList<QWebSocket *> clients;
};

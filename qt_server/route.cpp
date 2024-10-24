#include "route.h"
#include "database.h"


Router::Router(QObject *parent) : QObject(parent) {
    authHandler = new Auth(this);
    chatHandler = new Chat(this);
    connect(authHandler, &Auth::routeRequest, this, &Router::routeHttpRequest);
}

void Router::start() {
    // HTTP 서버 및 WebSocket 서버 시작
    if (!Database::instance().connectToDatabase("localhost", "mydb.sqlite", "", "")) {
        qDebug() << "Failed to connect to database. Server will not start.";
        return;  // 데이터베이스 연결 실패 시 서버 시작을 중단
    }

    // HTTP 서버 및 WebSocket 서버 시작
    qDebug() << "Starting HTTP and WebSocket servers...";

    authHandler->startServer(8080);  // HTTP 포트
    chatHandler->startServer(8081);  // WebSocket 포트
}

void Router::routeHttpRequest(QTcpSocket *socket, const QByteArray &request) {
    QString requestStr = QString::fromUtf8(request);
    if (requestStr.startsWith("POST /login")) {
        // 로그인 요청이면 Auth로 전달
        authHandler->handleLogin(socket, request);
    } else if (requestStr.startsWith("POST /register")) {
        authHandler->handleRegister(socket, request);
    } else {
        // 다른 HTTP 요청 처리 가능
        qDebug() << "Unsupported HTTP request";
        authHandler->sendErrorResponse(socket, "Unsupported request");
    }
}

void Router::routeWebSocketRequest(QWebSocket *socket, const QString &message) {
    chatHandler->handleMessage(socket, message);  // 채팅 메시지 처리
}


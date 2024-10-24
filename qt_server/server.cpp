#include "server.h"

Server::Server(QObject *parent) : QObject(parent) {
    router = new Router(this);
}

void Server::start() {
    router->start();  // 서버 시작 및 라우팅 시작
}

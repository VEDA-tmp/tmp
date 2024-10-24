#pragma once
#include <QObject>
#include "route.h"

class Server : public QObject {
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    void start();

private:
    Router *router;
};

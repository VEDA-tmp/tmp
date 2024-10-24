#pragma once
#include <QString>
#include <QDatetime>

QString get_currentTime() {
    QString current_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    return current_time;
}
#pragma once
#include <QObject>
#include "logviewer_view.h"
#include "logmodel.h"

class LogViewerController : public QObject {
    Q_OBJECT

public:
    LogViewerController(LogModel* model, LogViewerView* view, QObject* parent = nullptr)
        : QObject(parent), model(model), view(view) {
        view->setModel(model); // Set the model in the view
        model->loadAllLogs();     // Load initial logs

        connect(model, &LogModel::logAdded, this, &LogViewerController::onLogAdded);
    }

private slots:
    // 로그가 추가될 때 view를 갱신하는 슬롯
    void onLogAdded() {
        qDebug() << "onLogAdded";
        view->updateTableView();
    }

private:
    LogModel* model;
    LogViewerView* view;
};


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
        model->loadLogs();     // Load initial logs
    }

private:
    LogModel* model;
    LogViewerView* view;
};


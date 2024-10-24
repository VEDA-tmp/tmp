#include <QApplication>
#include "server.h"
#include "logviewer_view.h"
#include "logmodel.h"
#include "logviewer_controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // QCoreApplication a(argc, argv);
    Server server;
    server.start();

    // Model
    LogModel& model = LogModel::instance();

    model.addLog("Info", "2024-10-25 10:15:00", "production", "Log entry 1");
    model.addLog("Error", "2024-10-25 10:20:00", "development", "Log entry 2");
    model.addLog("Debug", "2024-10-25 10:25:00", "testing", "Log entry 3");

    // View
    LogViewerView logViewerView;
    logViewerView.setWindowTitle("Log Viewer");
    logViewerView.resize(800, 600);
    logViewerView.show();

    // Controller
    LogViewerController controller(&model, &logViewerView);
    model.loadAllLogs();

    logViewerView.show();

    return app.exec();
}
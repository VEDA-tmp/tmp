#include <QApplication>
#include "logviewer_view.h"
#include "logmodel.h"
#include "logviewer_controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // QCoreApplication a(argc, argv);

    // Server server;
    // server.start();

    // Model
    LogModel logModel;

    // View
    LogViewerView logViewerView;
    logViewerView.setWindowTitle("Log Viewer");
    logViewerView.resize(800, 600);
    logViewerView.show();

    // Controller
    LogViewerController controller(&logModel, &logViewerView);

    return app.exec();
}
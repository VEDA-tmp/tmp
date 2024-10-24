#include <QApplication>
#include "LoginWindowLayout.h"
#include "MainWindowLayout.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;

    LoginWindow loginWindow(&mainWindow);
    loginWindow.show();

    return app.exec();
}
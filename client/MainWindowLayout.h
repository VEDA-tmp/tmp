#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QStackedWidget>

class MainWindow : public QWidget {
    Q_OBJECT  // Qt 메타 시스템을 위한 매크로

public:
    MainWindow(QWidget *parent = nullptr);
    void switchToMainWindowLayout() {
        // MainWindowLayout으로 전환하는 로직
        stackedWidget->setCurrentWidget(mainWidget);
    }
    

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void sendChatMessage();

private:
    QTextEdit *chatArea;
    QTextEdit *chatInput;
    QStackedWidget *stackedWidget;
    QWidget *mainWidget;
};

#endif // MAINWINDOW_H

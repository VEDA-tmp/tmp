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

class MainWindow : public QWidget {
    Q_OBJECT  // Qt 메타 시스템을 위한 매크로

public:
    MainWindow(QWidget *parent = nullptr);
    

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void sendChatMessage();

private:
    QTextEdit *chatArea;
    QTextEdit *chatInput;
};

#endif // MAINWINDOW_H

#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

#include "MainWindowLayout.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(MainWindow *mainWindow, QWidget *parent = nullptr) 
        : QWidget(parent), mainWindow(mainWindow) {
        setupUI();

    // 네트워크 매니저 초기화
        networkManager = new QNetworkAccessManager(this);

        // 로그인 버튼 클릭 시 로그인 요청 전송
        connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
        connect(signupConfirmButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    }

private:
    QStackedWidget *stackedWidget;
    MainWindow *mainWindow;
    
    // 로그인 UI 요소
    QWidget *loginWidget;
    QLabel *logoLabel;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *signupButton;
    QLabel *forgotPasswordLabel;

    // 회원가입 UI 요소
    QWidget *signupWidget;
    QLabel *signupTitleLabel;
    QLineEdit *signupUsernameInput;
    QLineEdit *signupPasswordInput;
    QLineEdit *signupEmailInput;
    QPushButton *signupConfirmButton;
    QPushButton *loginLinkButton;
    QNetworkAccessManager *networkManager;  // 네트워크 매니저

    void setupUI();
    void setupLoginUI();
    void setupSignupUI();
    

private slots:
    void switchToSignup();
    void switchToLogin();
    void switchToMain();
    void sendAuthRequest(bool isLogin);  // 로그인 요청을 서버에 보내는 함수
    void handleAuthResponse(QNetworkReply *reply, bool islogin);  // 서버 응답 처리 함수
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    // void handleRegisterResponse(QNetworkReply *reply);  // 서버 응답 처리 함수
};

#endif // LOGINWINDOW_H

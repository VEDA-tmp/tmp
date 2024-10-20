#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    
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

    void setupUI();
    void setupLoginUI();
    void setupSignupUI();

private slots:
    void switchToSignup();
    void switchToLogin();
};

#endif // LOGINWINDOW_H

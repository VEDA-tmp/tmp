#include "LoginWindowLayout.h"
#include <QNetworkReplay>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void LoginWindow::setupUI() {
    // QStackedWidget 사용하여 로그인과 회원가입 UI 전환
    stackedWidget = new QStackedWidget(this);

    setupLoginUI();
    setupSignupUI();

    stackedWidget->setFixedSize(350, 500);
    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(signupWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);

    setLayout(mainLayout);
}

// 로그인 UI 설정
void LoginWindow::setupLoginUI() {
    loginWidget = new QWidget(this);

    // 로고
    logoLabel = new QLabel("TMP", this);
    logoLabel->setStyleSheet("color: white; font-size: 32px;");
    logoLabel->setAlignment(Qt::AlignCenter);

    // 사용자명 입력 필드
    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("User name");
    usernameInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    usernameInput->setFixedHeight(40);

    // 비밀번호 입력 필드
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    passwordInput->setFixedHeight(40);

    // 로그인 버튼
    loginButton = new QPushButton("Log In", this);
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 20px;");
    loginButton->setFixedHeight(50);

    // 회원가입으로 이동하는 버튼
    signupButton = new QPushButton("Sign Up", this);
    signupButton->setStyleSheet("background-color: transparent; color: #4CAF50; border: 1px solid #4CAF50; border-radius: 20px;");
    signupButton->setFixedHeight(50);
    connect(signupButton, &QPushButton::clicked, this, &LoginWindow::switchToSignup);  // 회원가입 UI로 전환

    // 비밀번호 찾기 레이블
    forgotPasswordLabel = new QLabel("<a href='#'>Forgot password?</a>", this);
    forgotPasswordLabel->setStyleSheet("color: #4CAF50;");
    forgotPasswordLabel->setAlignment(Qt::AlignCenter);
    forgotPasswordLabel->setTextFormat(Qt::RichText);
    forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotPasswordLabel->setOpenExternalLinks(true);

    // 로그인 UI 레이아웃 구성
    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);
    loginLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    loginLayout->addSpacing(20);
    loginLayout->addWidget(usernameInput);
    loginLayout->addWidget(passwordInput);
    loginLayout->addSpacing(20);
    loginLayout->addWidget(loginButton);
    loginLayout->addSpacing(20);
    loginLayout->addWidget(signupButton);
    loginLayout->addWidget(forgotPasswordLabel, 0, Qt::AlignCenter);

    loginWidget->setLayout(loginLayout);
}

// 회원가입 UI 설정
void LoginWindow::setupSignupUI() {
    signupWidget = new QWidget(this);

        // 이메일 입력 필드
    signupEmailInput = new QLineEdit(this);
    signupEmailInput->setPlaceholderText("Email");
    signupEmailInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    signupEmailInput->setFixedHeight(40);


    // 사용자명 입력 필드
    signupUsernameInput = new QLineEdit(this);
    signupUsernameInput->setPlaceholderText("User name");
    signupUsernameInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    signupUsernameInput->setFixedHeight(40);

    // 비밀번호 입력 필드
    signupPasswordInput = new QLineEdit(this);
    signupPasswordInput->setPlaceholderText("Password");
    signupPasswordInput->setEchoMode(QLineEdit::Password);
    signupPasswordInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    signupPasswordInput->setFixedHeight(40);

    // 회원가입 확인 버튼
    signupConfirmButton = new QPushButton("Sign Up", this);
    signupConfirmButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 20px;");
    signupConfirmButton->setFixedHeight(50);

    // 로그인으로 돌아가는 버튼
    loginLinkButton = new QPushButton("Log In", this);
    loginLinkButton->setStyleSheet("background-color: transparent; color: #4CAF50; border: 1px solid #4CAF50; border-radius: 20px;");
    loginLinkButton->setFixedHeight(50);
    connect(loginLinkButton, &QPushButton::clicked, this, &LoginWindow::switchToLogin);  // 로그인 UI로 전환

    // 회원가입 UI 레이아웃 구성
    QVBoxLayout *signupLayout = new QVBoxLayout(signupWidget);
    signupLayout->addWidget(signupEmailInput);
    signupLayout->addWidget(signupUsernameInput);
    signupLayout->addWidget(signupPasswordInput);
    signupLayout->addSpacing(20);
    signupLayout->addWidget(signupConfirmButton);
    signupLayout->addSpacing(10);
    signupLayout->addWidget(loginLinkButton);

    signupWidget->setLayout(signupLayout);
}

// 로그인 UI로 전환
void LoginWindow::switchToLogin() {
    stackedWidget->setCurrentWidget(loginWidget);
}

// 회원가입 UI로 전환
void LoginWindow::switchToSignup() {
    stackedWidget->setCurrentWidget(signupWidget);
}

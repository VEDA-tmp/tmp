#include "LoginWindowLayout.h"
#include <QMessageBox>


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

    logoLabel = new QLabel("TMP", this);
    logoLabel->setStyleSheet("color: white; font-size: 32px;");
    logoLabel->setAlignment(Qt::AlignCenter);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("User name");
    usernameInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    usernameInput->setFixedHeight(40);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("color: white; background-color: #2E3B4E; border: none; border-bottom: 1px solid #4CAF50; padding: 10px;");
    passwordInput->setFixedHeight(40);

    loginButton = new QPushButton("Log In", this);
    loginButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 20px;");
    loginButton->setFixedHeight(50);

    signupButton = new QPushButton("Sign Up", this);
    signupButton->setStyleSheet("background-color: transparent; color: #4CAF50; border: 1px solid #4CAF50; border-radius: 20px;");
    signupButton->setFixedHeight(50);
    connect(signupButton, &QPushButton::clicked, this, &LoginWindow::switchToSignup);

    forgotPasswordLabel = new QLabel("<a href='#'>Forgot password?</a>", this);
    forgotPasswordLabel->setStyleSheet("color: #4CAF50;");
    forgotPasswordLabel->setAlignment(Qt::AlignCenter);
    forgotPasswordLabel->setTextFormat(Qt::RichText);
    forgotPasswordLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    forgotPasswordLabel->setOpenExternalLinks(true);

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

void LoginWindow::sendAuthRequest(bool isLogin) {
    // 로그인일 경우 "/login", 회원가입일 경우 "/register"로 URL 설정
    QString endpoint = isLogin ? "http://localhost:8080/login" : "http://localhost:8080/register";
    QUrl url(endpoint);

    // JSON 요청 생성
    QJsonObject json;
    json["username"] = isLogin ? usernameInput->text() : signupUsernameInput->text();
    json["password"] = isLogin ? passwordInput->text() : signupPasswordInput->text();
    qDebug() << "username is : " << json["username"];
    qDebug() << "password is : " << json["password"];
    
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // 네트워크 요청 생성
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // POST 요청 전송
    QNetworkReply *reply = networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply, isLogin]() {
        handleAuthResponse(reply, isLogin);
    });
}


// 서버의 응답 처리
void LoginWindow::handleAuthResponse(QNetworkReply *reply, bool isLogin) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject["status"] == "200" && isLogin) {
            // 로그인 성공 처리
            QMessageBox::about(this, "로그인 성공", "로그인에 성공하셨습니다!");
            this->switchToMain();
            
        } else if (jsonObject["status"] == "200" && !isLogin) {
            // 로그인 실패 처리
            QMessageBox::about(this, "회원가입 성공", "회원가입에 성공하셨습니다!");
            this->switchToLogin();
        }
    } else {
        qDebug() << "Error in request:" << reply->errorString();
    }

    reply->deleteLater();  // 응답 객체 삭제
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

void LoginWindow::switchToMain() {
    mainWindow->switchToMainWindowLayout();
}

void LoginWindow::onLoginButtonClicked() {
    sendAuthRequest(true);  // 로그인 요청
}

// 회원가입 버튼 클릭 시 호출
void LoginWindow::onRegisterButtonClicked() {
    sendAuthRequest(false);  // 회원가입 요청
}
#include "MainWindowLayout.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    // 왼쪽 프레임 버퍼 (여기서는 QLabel로 대체)
    QLabel *frameBufferLabel = new QLabel("Frame Buffer Area");
    frameBufferLabel->setStyleSheet("background-color: black; color: white;"); // 배경색을 검정으로

    // 오른쪽 채팅 레이아웃
    chatArea = new QTextEdit();
    chatArea->setReadOnly(true);  // 읽기 전용
    chatInput = new QTextEdit();
    chatInput->setMaximumHeight(50);  // 입력 필드는 너무 크지 않게 설정

    QPushButton *sendButton = new QPushButton("Send");  // 보내기 버튼

    QVBoxLayout *chatLayout = new QVBoxLayout();
    chatLayout->addWidget(chatArea);  // 채팅 출력
    chatLayout->addWidget(chatInput);  // 채팅 입력
    chatLayout->addWidget(sendButton);  // 보내기 버튼

    // 전체 레이아웃: 왼쪽은 프레임 버퍼, 오른쪽은 채팅
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(frameBufferLabel);  // 왼쪽 프레임 버퍼
    mainLayout->addLayout(chatLayout);        // 오른쪽 채팅 레이아웃

    setLayout(mainLayout);
    setWindowTitle("Frame Buffer and Chat Layout Example");

    // 버튼 클릭 시 채팅 입력 처리
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendChatMessage);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        sendChatMessage();
    }
    QWidget::keyPressEvent(event);  // 기본 키 이벤트 처리
}

void MainWindow::sendChatMessage() {
    // IME 입력 중인 텍스트를 강제로 완료 처리
    chatInput->setPlainText(chatInput->toPlainText());

    // 입력 필드에서 텍스트 가져오기
    QString message = chatInput->toPlainText().trimmed();
    if (!message.isEmpty()) {
        // 채팅 출력 필드에 메시지 추가
        chatArea->append("User: " + message);
        // 입력 필드 비우기
        chatInput->clear();
    } else {
        QMessageBox::warning(this, "Warning", "Message is empty!");
    }
}

#pragma once
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>  // QHeaderView 헤더 파일 추가
#include <QListWidget>
#include "logmodel.h"

class LogViewerView : public QMainWindow {
    Q_OBJECT

public:
    LogViewerView(QWidget* parent = nullptr) : QMainWindow(parent) {
        // Central widget and main layout
        QWidget* centralWidget = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        setCentralWidget(centralWidget);

        // Top bar (Server selection, Search, Refresh button)
        QHBoxLayout* topBarLayout = new QHBoxLayout();
        QComboBox* serverComboBox = new QComboBox();
        serverComboBox->addItem("Local server");
        serverComboBox->addItem("Remote server");

        QLineEdit* searchEdit = new QLineEdit();
        searchEdit->setPlaceholderText("Search...");

        QPushButton* refreshButton = new QPushButton("Refresh");

        topBarLayout->addWidget(serverComboBox);
        topBarLayout->addWidget(searchEdit);
        topBarLayout->addWidget(refreshButton);
        mainLayout->addLayout(topBarLayout);

        // Log File List (Left panel)
        QHBoxLayout* mainContentLayout = new QHBoxLayout();
        menuList = new QListWidget();

        // 메뉴 항목 추가 (전체 및 특정 환경)
        menuList->addItem("All Logs");
        menuList->addItem("Auth");
        menuList->addItem("Chat");
        menuList->addItem("Image");

        // QListWidget을 메인 레이아웃에 추가
        mainContentLayout->addWidget(menuList, 1);

        // Log Entries Table (Right panel)
        logTableView = new QTableView();
        logTableView->horizontalHeader()->setStretchLastSection(true); // Stretch last column
        logTableView->verticalHeader()->setVisible(false); // Hide row numbers
        logTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        logTableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Non-editable
        mainContentLayout->addWidget(logTableView, 3); // Give more space to log entries

        mainLayout->addLayout(mainContentLayout);

        // 메뉴 선택 시 로그 필터링 처리
        connect(menuList, &QListWidget::itemClicked, this, &LogViewerView::onMenuItemClicked);
    }

    void setModel(LogModel* model) {
        logTableView->setModel(model);
        this->model = model;
        qDebug() << "setModel Activated..";
    }

    void updateTableView() {
        model->loadAllLogs();
        logTableView->viewport()->update();  // 뷰포트 업데이트 (갱신)
    }

private slots:
    // 메뉴 항목이 클릭되었을 때 처리하는 슬롯
    void onMenuItemClicked(QListWidgetItem* item) {
        QString selectedEnv = item->text();

        if (selectedEnv == "All Logs") {
            model->loadAllLogs();  // 전체 로그 불러오기
        } else {
            model->loadLogsByEnv(selectedEnv.toLower());  // 선택한 환경에 따른 로그 필터링
        }
    }

    

private:
    QTableView* logTableView;
    QListWidget* menuList;
    LogModel* model;
};

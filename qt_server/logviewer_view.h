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
        QListWidget* menuList = new QListWidget();

        // 메뉴 항목 추가
        menuList->addItem("User");
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

        // Footer
        // QHBoxLayout* footerLayout = new QHBoxLayout();
        // QLabel* memoryLabel = new QLabel("Memory: 6.00 MB");
        // QLabel* durationLabel = new QLabel("Duration: 80ms");
        // QLabel* versionLabel = new QLabel("Version: v2.1.0");

        // footerLayout->addWidget(memoryLabel);
        // footerLayout->addWidget(durationLabel);
        // footerLayout->addStretch(); // Spacer
        // footerLayout->addWidget(versionLabel);
        // mainLayout->addLayout(footerLayout);
    }

    void setModel(LogModel* model) {
        logTableView->setModel(model);
    }

private:
    QTableView* logTableView;
};

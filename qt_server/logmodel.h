#pragma once
#include <QObject>
#include <QAbstractTableModel>
#include <QVector>
#include <QString>

struct LogEntry {
    QString level;
    QString time;
    QString env;
    QString description;
};

class LogModel : public QAbstractTableModel {
    Q_OBJECT

public:
    LogModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    // Add some example log entries (replace with actual log loading logic)
    void loadLogs() {
        logs.append({"Debug", "2024-10-23 12:34:56", "production", "[UID 12345] Example log entry"});
        logs.append({"Info", "2024-10-23 12:35:56", "production", "[UID 12346] Another log entry"});
        logs.append({"Error", "2024-10-23 12:36:56", "development", "[UID 12347] Error log entry"});
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return logs.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4; // Level, Time, Env, Description
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const LogEntry& entry = logs.at(index.row());
        switch (index.column()) {
        case 0: return entry.level;
        case 1: return entry.time;
        case 2: return entry.env;
        case 3: return entry.description;
        default: return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return QVariant();

        switch (section) {
        case 0: return QString("Level");
        case 1: return QString("Time");
        case 2: return QString("Env");
        case 3: return QString("Description");
        default: return QVariant();
        }
    }

private:
    QVector<LogEntry> logs;
};

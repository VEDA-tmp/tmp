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
    // 싱글톤 인스턴스에 접근하는 정적 메서드
    static LogModel& instance() {
        static LogModel instance;
        return instance;
    }

    // 로그를 추가하는 메서드
    void addLog(const QString& level, const QString& time, const QString& env, const QString& description) {
        beginInsertRows(QModelIndex(), logs.size(), logs.size());
        logs.prepend({level, time, env, description});
        endInsertRows();
        emit logAdded();
    }

    // 특정 환경(env)에 맞는 로그를 불러오는 메서드
    void loadLogsByEnv(const QString& env) {
        beginResetModel();  // 모델 리셋 준비

        // logs를 필터링하여 해당 env에 맞는 로그만 남김
        filteredLogs.clear();
        for (const auto& log : logs) {
            if (log.env == env) {
                filteredLogs.append(log);
            }
        }

        endResetModel();  // 모델 리셋 완료
    }

    // 전체 로그를 불러오는 메서드
    void loadAllLogs() {
        beginResetModel();  // 모델 리셋 준비

        // 전체 로그를 표시하기 위해 filteredLogs를 logs와 동일하게 설정
        filteredLogs = logs;

        endResetModel();  // 모델 리셋 완료
    }

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        return filteredLogs.size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        return 4; // Level, Time, Env, Description
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

        const LogEntry& entry = filteredLogs.at(index.row());
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

signals:
    void logAdded();  // 로그가 추가될 때 발생하는 신호

private:
    LogModel(QObject* parent = nullptr) : QAbstractTableModel(parent) {}

    // 전체 로그를 저장하는 QVector
    QVector<LogEntry> logs;

    // 필터링된 로그만 보여줄 QVector
    QVector<LogEntry> filteredLogs;
};

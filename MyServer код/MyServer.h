#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer(QObject* parent = nullptr);
    void startServer();
    void startSession(QString topic);
    void startVoting();
    void finishSession();
    void kickUser(int index); // Видалення користувача

signals:
    void logMessage(QString msg);
    void timeUpdated(QString timeStr);
    void usersListChanged(QStringList names); // Оновлення списку GUI

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();
    void onTimerTick();

private:
    QList<QTcpSocket*> clients;
    QMap<QTcpSocket*, QString> clientNames;
    QStringList ideas;
    QMap<QString, int> votes;

    QTimer* timer;
    int timeLeft;
    QString currentTopic;

    void sendToAll(const QJsonObject& json);
    void saveReportToFile(QString resultText);
    void updateUserList();
};
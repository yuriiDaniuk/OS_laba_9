#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QTimer>   // <--- ДОДАЛИ ТАЙМЕР
#include <QFile>    // <--- ДОДАЛИ ФАЙЛИ
#include <QTextStream>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer(QObject* parent = nullptr);
    void startServer();
    void startSession(QString topic); // <--- Тепер приймаємо тему
    void startVoting();
    void finishSession();

signals:
    void logMessage(QString msg);
    void timeUpdated(QString timeStr); // <--- Сигнал, щоб оновлювати час на екрані Адміна

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();
    void onTimerTick(); // <--- Що робити кожну секунду

private:
    QList<QTcpSocket*> clients;
    QMap<QTcpSocket*, QString> clientNames;
    QStringList ideas;
    QMap<QString, int> votes;

    QTimer* timer;         // <--- Таймер
    int timeLeft;          // <--- Скільки секунд залишилось
    QString currentTopic;  // <--- Поточна тема

    void sendToAll(const QJsonObject& json);
    void saveReportToFile(QString resultText); // <--- Функція збереження
};
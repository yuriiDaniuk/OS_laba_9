#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MyClient : public QObject
{
    Q_OBJECT

public:
    explicit MyClient(QObject* parent = nullptr);

    void connectToServer(QString ip, int port, QString name);
    void sendIdea(QString ideaText);
    void sendVotes(QStringList selectedIdeas);
    bool isConnected(); // Перевірка чи є зв'язок

signals:
    void logMessage(QString msg);

    // --- НОВІ СИГНАЛИ ---
    void topicReceived(QString topic);     // Отримали тему
    void timerReceived(QString timeStr);   // Отримали час
    // --------------------

    void sessionStarted();
    void votingStarted(QStringList ideas);
    void resultsReceived(QString results);

private slots:
    void onReadyRead();

private:
    QTcpSocket* socket;
};
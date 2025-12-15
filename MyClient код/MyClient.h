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
    bool isConnected();

signals:
    void logMessage(QString msg);
    void topicReceived(QString topic);
    void timerReceived(QString timeStr);
    void sessionStarted();
    void votingStarted(QStringList ideas);
    void resultsReceived(QString results);

    // --- ÕŒ¬»… —»√Õ¿À ---
    void ideaReceived(QString idea);
    // --------------------

private slots:
    void onReadyRead();

private:
    QTcpSocket* socket;
};
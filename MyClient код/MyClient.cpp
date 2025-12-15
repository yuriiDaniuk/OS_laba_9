#include "MyClient.h"

MyClient::MyClient(QObject* parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MyClient::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, [this]() {
        emit logMessage("Connected to server!");
        });
    connect(socket, &QTcpSocket::disconnected, this, [this]() {
        emit logMessage("Disconnected from server.");
        });
}

void MyClient::connectToServer(QString ip, int port, QString name)
{
    socket->connectToHost(ip, port);
    if (socket->waitForConnected(3000)) {
        QJsonObject json;
        json["type"] = "login";
        json["name"] = name;
        socket->write(QJsonDocument(json).toJson());
    }
    else {
        emit logMessage("Error: Server not found.");
    }
}

bool MyClient::isConnected() {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void MyClient::sendIdea(QString ideaText)
{
    if (ideaText.isEmpty()) return;
    QJsonObject json;
    json["type"] = "idea";
    json["text"] = ideaText;
    socket->write(QJsonDocument(json).toJson());
    // Ми не пишемо emit logMessage тут, бо сервер поверне нам цю ідею через broadcast
}

void MyClient::sendVotes(QStringList selectedIdeas)
{
    QJsonObject json;
    json["type"] = "vote";
    QJsonArray arr;
    for (const QString& idea : selectedIdeas) arr.append(idea);
    json["votes"] = arr;
    socket->write(QJsonDocument(json).toJson());
    emit logMessage("Votes sent.");
}

void MyClient::onReadyRead()
{
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();
    QString type = json["type"].toString();

    if (type == "start_session") {
        QString topic = json["topic"].toString();
        emit topicReceived(topic);
        emit sessionStarted();
    }
    else if (type == "timer") {
        QString timeStr = json["time"].toString();
        emit timerReceived(timeStr);
    }
    // --- ОСНОВНА ЗМІНА ТУТ ---
    else if (type == "idea_broadcast") {
        QString txt = json["text"].toString();
        QString author = json["author"].toString();

        // 1. Пишемо в лог (знизу)
        emit logMessage("💡 " + author + ": " + txt);

        // 2. Відправляємо текст у центральний список
        emit ideaReceived(txt);
    }
    // -------------------------
    else if (type == "start_voting") {
        emit logMessage("Time's up! Voting started.");
        QStringList ideasList;
        QJsonArray arr = json["ideas"].toArray();
        for (auto i : arr) ideasList.append(i.toString());
        emit votingStarted(ideasList);
    }
    else if (type == "results") {
        QString res = json["text"].toString();
        emit logMessage("Results received.");
        emit resultsReceived(res);
    }
}
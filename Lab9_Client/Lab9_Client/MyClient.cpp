#include "MyClient.h"

MyClient::MyClient(QObject* parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MyClient::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, [this]() {
        emit logMessage("Підключено до сервера!");
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
        emit logMessage("Помилка! Сервер не знайдено.");
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
    emit logMessage("Ви: " + ideaText);
}

void MyClient::sendVotes(QStringList selectedIdeas)
{
    QJsonObject json;
    json["type"] = "vote";
    QJsonArray arr;
    for (const QString& idea : selectedIdeas) arr.append(idea);
    json["votes"] = arr;
    socket->write(QJsonDocument(json).toJson());
    emit logMessage("Голос відправлено.");
}

void MyClient::onReadyRead()
{
    QByteArray data = socket->readAll();
    // Сервер може надсилати кілька пакетів разом, тому розбиваємо їх (спрощено)
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();
    QString type = json["type"].toString();

    if (type == "start_session") {
        QString topic = json["topic"].toString();
        emit topicReceived(topic); // <--- Передаємо тему у вікно
        emit logMessage("Початок! Тема: " + topic);
        emit sessionStarted();
    }
    else if (type == "timer") {
        QString timeStr = json["time"].toString();
        emit timerReceived(timeStr); // <--- Передаємо час у вікно
        // НЕ пишемо в лог, щоб не спамити
    }
    else if (type == "start_voting") {
        emit logMessage("Час вийшов! Голосування.");
        QStringList ideasList;
        QJsonArray arr = json["ideas"].toArray();
        for (auto i : arr) ideasList.append(i.toString());
        emit votingStarted(ideasList);
    }
    else if (type == "results") {
        QString res = json["text"].toString();
        emit logMessage("Отримано результати.");
        emit resultsReceived(res);
    }
}
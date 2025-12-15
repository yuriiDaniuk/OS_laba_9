#include "MyServer.h"
#include <algorithm> // Для сортування

MyServer::MyServer(QObject* parent) : QTcpServer(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyServer::onTimerTick);
}

void MyServer::startServer() {
    if (this->listen(QHostAddress::Any, 2323)) {
        emit logMessage("SERVER STARTED! Port: 2323");
    }
    else {
        emit logMessage("Error: Could not start server.");
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &MyServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyServer::onDisconnected);
    clients.append(socket);
    emit logMessage("New connection established...");
}

void MyServer::onReadyRead() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    QByteArray data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();
    QString type = json["type"].toString();

    if (type == "login") {
        QString name = json["name"].toString();
        clientNames[socket] = name;
        emit logMessage("Login: " + name);
        updateUserList();
    }
    else if (type == "idea") {
        QString idea = json["text"].toString();
        QString author = clientNames[socket]; // Хто написав?

        ideas.append(idea);
        votes[idea] = 0;

        // 1. Показуємо Адміну
        emit logMessage("💡 Ідея від " + author + ": " + idea);

        // 2. --- НОВЕ: ВІДПРАВЛЯЄМО ВСІМ СТУДЕНТАМ МИТТЄВО ---
        QJsonObject broadcast;
        broadcast["type"] = "idea_broadcast"; // Новий тип повідомлення
        broadcast["text"] = idea;
        broadcast["author"] = author;
        sendToAll(broadcast);
        // ----------------------------------------------------
    }
    else if (type == "vote") {
        QJsonArray myVotes = json["votes"].toArray();
        for (auto v : myVotes) votes[v.toString()]++;
        emit logMessage("Vote received from " + clientNames[socket]);
    }
}

void MyServer::onDisconnected() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString name = clientNames[socket];
    clients.removeAll(socket);
    clientNames.remove(socket);
    socket->deleteLater();

    if (!name.isEmpty()) {
        emit logMessage("Disconnected: " + name);
        updateUserList();
    }
}

void MyServer::kickUser(int index) {
    if (index < 0 || index >= clients.size()) return;
    QTcpSocket* socketToKick = clients.at(index);
    QString name = clientNames[socketToKick];
    emit logMessage("KICKED BY ADMIN: " + name);
    socketToKick->disconnectFromHost();
}

void MyServer::updateUserList() {
    QStringList names;
    for (QTcpSocket* socket : clients) {
        if (clientNames.contains(socket)) names.append(clientNames[socket]);
        else names.append("Unknown (connecting...)");
    }
    emit usersListChanged(names);
}

void MyServer::startSession(QString topic) {
    ideas.clear();
    votes.clear();
    currentTopic = topic;
    timeLeft = 180; // 3 хвилини
    timer->start(1000);

    QJsonObject json;
    json["type"] = "start_session";
    json["topic"] = topic;
    sendToAll(json);
    emit logMessage("--- SESSION STARTED: " + topic + " ---");
}

void MyServer::onTimerTick() {
    timeLeft--;
    int min = timeLeft / 60;
    int sec = timeLeft % 60;
    QString timeStr = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    emit timeUpdated(timeStr);

    QJsonObject json;
    json["type"] = "timer";
    json["time"] = timeStr;
    sendToAll(json);

    if (timeLeft <= 0) {
        timer->stop();
        startVoting();
    }
}

void MyServer::startVoting() {
    timer->stop();
    QJsonObject json;
    json["type"] = "start_voting";
    QJsonArray ideasArray;
    for (const QString& idea : ideas) ideasArray.append(idea);
    json["ideas"] = ideasArray;
    sendToAll(json);
    emit logMessage("--- VOTING STARTED ---");
}

void MyServer::finishSession() {
    QList<QPair<QString, int>> sortedList;
    auto keys = votes.keys();
    for (const QString& key : keys) {
        sortedList.append(qMakePair(key, votes[key]));
    }

    std::sort(sortedList.begin(), sortedList.end(), [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
        return a.second > b.second;
        });

    QString resultStr = "--- FINAL RESULTS (" + currentTopic + ") ---\n\n";
    resultStr += "🏆 TOP-3 IDEAS:\n";

    for (int i = 0; i < sortedList.size(); ++i) {
        if (i < 3) {
            resultStr += QString::number(i + 1) + ". " + sortedList[i].first +
                " (" + QString::number(sortedList[i].second) + " votes)\n";
        }
        else {
            if (i == 3) resultStr += "\n--- Other Ideas ---\n";
            resultStr += sortedList[i].first + " (" + QString::number(sortedList[i].second) + " votes)\n";
        }
    }

    QJsonObject json;
    json["type"] = "results";
    json["text"] = resultStr;
    sendToAll(json);

    emit logMessage(resultStr);
    saveReportToFile(resultStr);
}

void MyServer::saveReportToFile(QString resultText) {
    QFile file("Report.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString() << "\n";
        out << resultText << "\n-------------------\n";
        file.close();
        emit logMessage("Report saved to Report.txt");
    }
}

void MyServer::sendToAll(const QJsonObject& json) {
    QByteArray data = QJsonDocument(json).toJson();
    for (QTcpSocket* socket : clients) socket->write(data);
}
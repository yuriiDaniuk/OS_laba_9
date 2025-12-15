#include "Lab9_Server.h"

Lab9_Server::Lab9_Server(QWidget* parent) : QMainWindow(parent)
{
    this->setWindowTitle("Server (Admin Panel)");
    this->resize(650, 500);
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Тема
    mainLayout->addWidget(new QLabel("Topic:"));
    topicInput = new QLineEdit(this);
    topicInput->setPlaceholderText("Enter topic...");
    mainLayout->addWidget(topicInput);

    // Таймер
    timerLabel = new QLabel("03:00", this);
    timerLabel->setObjectName("timerLabel"); // Для CSS
    timerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(timerLabel);

    // Середня частина
    QHBoxLayout* midLayout = new QHBoxLayout();

    QVBoxLayout* usersLayout = new QVBoxLayout();
    usersLayout->addWidget(new QLabel("Participants:"));
    userList = new QListWidget(this);
    usersLayout->addWidget(userList);
    btnKick = new QPushButton("KICK USER", this);
    btnKick->setObjectName("btnKick"); // Для CSS (червона)
    usersLayout->addWidget(btnKick);
    midLayout->addLayout(usersLayout, 1);

    QVBoxLayout* logLayout = new QVBoxLayout();
    logLayout->addWidget(new QLabel("Logs:"));
    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logLayout->addWidget(logOutput);
    midLayout->addLayout(logLayout, 2);

    mainLayout->addLayout(midLayout);

    // Кнопки
    btnStart = new QPushButton("Start Session (Restart)", this);
    mainLayout->addWidget(btnStart);
    btnVote = new QPushButton("Start Voting", this);
    mainLayout->addWidget(btnVote);
    btnResults = new QPushButton("Show Results", this);
    mainLayout->addWidget(btnResults);

    // Логіка
    server = new MyServer(this);

    connect(server, &MyServer::logMessage, this, [this](QString msg) {
        logOutput->append(msg);
        });
    connect(server, &MyServer::timeUpdated, this, [this](QString timeStr) {
        timerLabel->setText(timeStr);
        });
    connect(server, &MyServer::usersListChanged, this, [this](QStringList names) {
        userList->clear();
        userList->addItems(names);
        });

    // --- ОСЬ ТУТ ЗМІНИ (ОЧИЩЕННЯ) ---
    connect(btnStart, &QPushButton::clicked, this, [this]() {
        QString topic = topicInput->text();
        if (topic.isEmpty()) topic = "Brainstorming";

        // Очищаємо екран Адміна
        logOutput->clear();
        timerLabel->setText("03:00");

        server->startSession(topic);
        });

    connect(btnKick, &QPushButton::clicked, this, [this]() {
        int row = userList->currentRow();
        if (row >= 0) server->kickUser(row);
        else QMessageBox::warning(this, "Error", "Select a user to kick.");
        });

    connect(btnVote, &QPushButton::clicked, server, &MyServer::startVoting);
    connect(btnResults, &QPushButton::clicked, server, &MyServer::finishSession);

    server->startServer();
}

Lab9_Server::~Lab9_Server() {}
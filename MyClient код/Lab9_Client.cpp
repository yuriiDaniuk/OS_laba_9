#include "Lab9_Client.h"

Lab9_Client::Lab9_Client(QWidget* parent) : QMainWindow(parent)
{
    this->setWindowTitle("Student Client");
    this->resize(450, 700);
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Підключення
    QHBoxLayout* connLayout = new QHBoxLayout();
    ipInput = new QLineEdit("127.0.0.1", this);
    ipInput->setPlaceholderText("IP Address");
    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Your Name");
    btnConnect = new QPushButton("Connect", this);
    connLayout->addWidget(new QLabel("IP:"));
    connLayout->addWidget(ipInput);
    connLayout->addWidget(nameInput);
    connLayout->addWidget(btnConnect);
    mainLayout->addLayout(connLayout);

    // Інфо
    topicLabel = new QLabel("Waiting for topic...", this);
    topicLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    topicLabel->setAlignment(Qt::AlignCenter);
    topicLabel->setWordWrap(true);
    mainLayout->addWidget(topicLabel);

    timerLabel = new QLabel("--:--", this);
    timerLabel->setObjectName("timerLabel"); // Для CSS
    timerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(timerLabel);

    // Ідеї
    mainLayout->addWidget(new QLabel("Your Idea:"));
    ideaInput = new QLineEdit(this);
    ideaInput->setPlaceholderText("Type here...");
    ideaInput->setEnabled(false);
    btnSend = new QPushButton("Send Idea", this);
    btnSend->setEnabled(false);
    mainLayout->addWidget(ideaInput);
    mainLayout->addWidget(btnSend);

    // Голосування (Центральний список)
    mainLayout->addWidget(new QLabel("Vote (Max 3):"));
    votingList = new QListWidget(this);
    votingList->setSelectionMode(QAbstractItemView::MultiSelection);
    btnVote = new QPushButton("Submit Vote", this);
    btnVote->setEnabled(false);
    mainLayout->addWidget(votingList);
    mainLayout->addWidget(btnVote);

    // Лог
    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(100);
    mainLayout->addWidget(logOutput);

    // Логіка
    client = new MyClient(this);

    connect(client, &MyClient::logMessage, this, [this](QString msg) {
        logOutput->append(msg);
        });

    // --- НОВЕ: Як тільки приходить ідея -> додаємо її в список
    connect(client, &MyClient::ideaReceived, this, [this](QString idea) {
        votingList->addItem(idea);
        });
    // --------------------------------------------------------

    connect(client, &MyClient::topicReceived, this, [this](QString topic) {
        topicLabel->setText(topic);
        });
    connect(client, &MyClient::timerReceived, this, [this](QString time) {
        timerLabel->setText(time);
        });

    connect(btnConnect, &QPushButton::clicked, this, [this]() {
        QString name = nameInput->text().trimmed();
        QString ip = ipInput->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter your name!");
            return;
        }
        btnConnect->setEnabled(false);
        nameInput->setEnabled(false);
        ipInput->setEnabled(false);
        client->connectToServer(ip, 2323, name);
        });

    connect(btnSend, &QPushButton::clicked, this, [this]() {
        if (!client->isConnected()) {
            QMessageBox::critical(this, "Error", "Not connected!");
            return;
        }
        QString text = ideaInput->text().trimmed();
        if (!text.isEmpty()) {
            client->sendIdea(text);
            ideaInput->clear();
        }
        });

    connect(btnVote, &QPushButton::clicked, this, [this]() {
        QStringList selected;
        for (auto item : votingList->selectedItems()) selected.append(item->text());

        if (selected.size() > 3) {
            QMessageBox::warning(this, "Rules", "You can select MAX 3 ideas!");
            return;
        }
        if (selected.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Select at least 1 idea.");
            return;
        }

        client->sendVotes(selected);
        btnVote->setEnabled(false);
        votingList->setEnabled(false); // Блокуємо список після голосування
        topicLabel->setText("Vote Submitted.");
        });

    // ПОВНЕ ОЧИЩЕННЯ ПРИ НОВІЙ СЕСІЇ
    connect(client, &MyClient::sessionStarted, this, [this]() {
        logOutput->clear();
        votingList->clear();
        ideaInput->clear();

        timerLabel->setText("03:00");

        ideaInput->setEnabled(true);
        btnSend->setEnabled(true);
        btnVote->setEnabled(false);
        votingList->setEnabled(true); // Розблокуємо список для нових ідей

        ideaInput->setFocus();
        logOutput->append("--- NEW SESSION STARTED ---");
        });

    connect(client, &MyClient::votingStarted, this, [this](QStringList ideas) {
        ideaInput->setEnabled(false);
        btnSend->setEnabled(false);

        // Тут ми перезаписуємо список, щоб у всіх був однаковий порядок ідей (за бажанням)
        // Або залишаємо як є, бо ідеї вже додані.
        // Але сервер надсилає фінальний список, тому краще оновити:
        votingList->clear();
        votingList->addItems(ideas);

        btnVote->setEnabled(true);
        });

    connect(client, &MyClient::resultsReceived, this, [this](QString res) {
        QMessageBox::information(this, "Final Results", res);
        });
}

Lab9_Client::~Lab9_Client() {}
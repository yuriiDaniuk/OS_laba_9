#include "Lab9_Client.h"

Lab9_Client::Lab9_Client(QWidget* parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Student Client");
    this->resize(450, 700);

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // 1. ПІДКЛЮЧЕННЯ
    QHBoxLayout* connLayout = new QHBoxLayout();
    ipInput = new QLineEdit("127.0.0.1", this);
    ipInput->setPlaceholderText("IP Address");
    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Ваше Ім'я");
    btnConnect = new QPushButton("Вхід", this);

    connLayout->addWidget(new QLabel("IP:"));
    connLayout->addWidget(ipInput);
    connLayout->addWidget(nameInput);
    connLayout->addWidget(btnConnect);
    mainLayout->addLayout(connLayout);

    // 2. ІНФОРМАЦІЙНА ПАНЕЛЬ (ТЕМА і ТАЙМЕР)
    topicLabel = new QLabel("Очікування теми...", this);
    topicLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");
    topicLabel->setWordWrap(true); // Перенос слів
    topicLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(topicLabel);

    timerLabel = new QLabel("--:--", this);
    timerLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #e74c3c;");
    timerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(timerLabel);

    // 3. ІДЕЇ
    mainLayout->addWidget(new QLabel("Ваша ідея:"));
    ideaInput = new QLineEdit(this);
    ideaInput->setPlaceholderText("Напишіть ідею...");
    ideaInput->setEnabled(false); // Спочатку заблоковано

    btnSend = new QPushButton("Надіслати", this);
    btnSend->setEnabled(false);   // Спочатку заблоковано

    mainLayout->addWidget(ideaInput);
    mainLayout->addWidget(btnSend);

    // 4. ГОЛОСУВАННЯ
    mainLayout->addWidget(new QLabel("Голосування:"));
    votingList = new QListWidget(this);
    votingList->setSelectionMode(QAbstractItemView::MultiSelection);
    btnVote = new QPushButton("Проголосувати", this);
    btnVote->setEnabled(false);

    mainLayout->addWidget(votingList);
    mainLayout->addWidget(btnVote);

    // 5. ЛОГ
    logOutput = new QTextEdit(this);
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(100); // Зменшимо лог, він тепер менш важливий
    mainLayout->addWidget(logOutput);


    // --- ЛОГІКА ---
    client = new MyClient(this);

    connect(client, &MyClient::logMessage, this, [this](QString msg) {
        logOutput->append(msg);
        });

    // Оновлення Теми
    connect(client, &MyClient::topicReceived, this, [this](QString topic) {
        topicLabel->setText(topic);
        });

    // Оновлення Таймера
    connect(client, &MyClient::timerReceived, this, [this](QString time) {
        timerLabel->setText(time);
        });


    // --- КНОПКИ З ПЕРЕВІРКАМИ ---

    // Кнопка Вхід
    connect(btnConnect, &QPushButton::clicked, this, [this]() {
        QString name = nameInput->text().trimmed();
        QString ip = ipInput->text().trimmed();

        // ПЕРЕВІРКА 1: Чи ввів ім'я?
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Помилка", "Будь ласка, введіть своє ім'я!");
            return;
        }

        // Блокуємо поля вводу після натискання
        btnConnect->setEnabled(false);
        nameInput->setEnabled(false);
        ipInput->setEnabled(false);

        client->connectToServer(ip, 2323, name);
        });

    // Кнопка Надіслати
    connect(btnSend, &QPushButton::clicked, this, [this]() {
        // ПЕРЕВІРКА 2: Чи підключений?
        if (!client->isConnected()) {
            QMessageBox::critical(this, "Помилка", "Немає з'єднання з сервером!");
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

        if (selected.isEmpty()) {
            QMessageBox::warning(this, "Увага", "Виберіть хоча б одну ідею!");
            return;
        }

        client->sendVotes(selected);
        btnVote->setEnabled(false);
        votingList->setEnabled(false);
        topicLabel->setText("Дякуємо за голос!");
        });


    // --- УПРАВЛІННЯ СТАНОМ ---

    // Коли сесія почалася -> Розблокувати ввід ідей
    connect(client, &MyClient::sessionStarted, this, [this]() {
        ideaInput->setEnabled(true);
        btnSend->setEnabled(true);
        ideaInput->setFocus();
        });

    // Коли голосування -> Блокувати ідеї, розблокувати список
    connect(client, &MyClient::votingStarted, this, [this](QStringList ideas) {
        ideaInput->setEnabled(false);
        btnSend->setEnabled(false);

        votingList->clear();
        votingList->addItems(ideas);
        btnVote->setEnabled(true);
        });

    connect(client, &MyClient::resultsReceived, this, [this](QString res) {
        logOutput->append("\n" + res);
        // Можна показати результати у спливаючому вікні для зручності
        QMessageBox::information(this, "Результати", res);
        });
}

Lab9_Client::~Lab9_Client() {}
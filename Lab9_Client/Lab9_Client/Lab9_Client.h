#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox> // <--- Для спливаючих вікон (помилок)
#include "MyClient.h"

class Lab9_Client : public QMainWindow
{
    Q_OBJECT

public:
    Lab9_Client(QWidget* parent = nullptr);
    ~Lab9_Client();

private:
    MyClient* client;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    // Верхня панель
    QLineEdit* ipInput;
    QLineEdit* nameInput;
    QPushButton* btnConnect;

    // Панель інформації (НОВЕ)
    QLabel* topicLabel;  // Тут буде тема
    QLabel* timerLabel;  // Тут буде час

    // Панель ідей
    QLineEdit* ideaInput;
    QPushButton* btnSend;

    // Панель голосування
    QListWidget* votingList;
    QPushButton* btnVote;

    QTextEdit* logOutput;
};
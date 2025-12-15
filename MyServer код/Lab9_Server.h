#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>
#include "MyServer.h"

class Lab9_Server : public QMainWindow
{
    Q_OBJECT

public:
    Lab9_Server(QWidget* parent = nullptr);
    ~Lab9_Server();

private:
    MyServer* server;
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    QLineEdit* topicInput;
    QLabel* timerLabel;

    QListWidget* userList;
    QTextEdit* logOutput;

    QPushButton* btnKick;
    QPushButton* btnStart;
    QPushButton* btnVote;
    QPushButton* btnResults;
};
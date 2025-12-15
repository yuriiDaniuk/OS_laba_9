/********************************************************************************
** Form generated from reading UI file 'Lab9_Server.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB9_SERVER_H
#define UI_LAB9_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Lab9_ServerClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Lab9_ServerClass)
    {
        if (Lab9_ServerClass->objectName().isEmpty())
            Lab9_ServerClass->setObjectName("Lab9_ServerClass");
        Lab9_ServerClass->resize(600, 400);
        menuBar = new QMenuBar(Lab9_ServerClass);
        menuBar->setObjectName("menuBar");
        Lab9_ServerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Lab9_ServerClass);
        mainToolBar->setObjectName("mainToolBar");
        Lab9_ServerClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Lab9_ServerClass);
        centralWidget->setObjectName("centralWidget");
        Lab9_ServerClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Lab9_ServerClass);
        statusBar->setObjectName("statusBar");
        Lab9_ServerClass->setStatusBar(statusBar);

        retranslateUi(Lab9_ServerClass);

        QMetaObject::connectSlotsByName(Lab9_ServerClass);
    } // setupUi

    void retranslateUi(QMainWindow *Lab9_ServerClass)
    {
        Lab9_ServerClass->setWindowTitle(QCoreApplication::translate("Lab9_ServerClass", "Lab9_Server", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lab9_ServerClass: public Ui_Lab9_ServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB9_SERVER_H

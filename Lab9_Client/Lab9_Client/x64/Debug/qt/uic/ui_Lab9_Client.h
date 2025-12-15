/********************************************************************************
** Form generated from reading UI file 'Lab9_Client.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB9_CLIENT_H
#define UI_LAB9_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Lab9_ClientClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Lab9_ClientClass)
    {
        if (Lab9_ClientClass->objectName().isEmpty())
            Lab9_ClientClass->setObjectName("Lab9_ClientClass");
        Lab9_ClientClass->resize(600, 400);
        menuBar = new QMenuBar(Lab9_ClientClass);
        menuBar->setObjectName("menuBar");
        Lab9_ClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Lab9_ClientClass);
        mainToolBar->setObjectName("mainToolBar");
        Lab9_ClientClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Lab9_ClientClass);
        centralWidget->setObjectName("centralWidget");
        Lab9_ClientClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Lab9_ClientClass);
        statusBar->setObjectName("statusBar");
        Lab9_ClientClass->setStatusBar(statusBar);

        retranslateUi(Lab9_ClientClass);

        QMetaObject::connectSlotsByName(Lab9_ClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *Lab9_ClientClass)
    {
        Lab9_ClientClass->setWindowTitle(QCoreApplication::translate("Lab9_ClientClass", "Lab9_Client", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lab9_ClientClass: public Ui_Lab9_ClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB9_CLIENT_H

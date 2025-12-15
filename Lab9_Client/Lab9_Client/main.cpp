#include "Lab9_Client.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab9_Client w;
    w.show();
    return a.exec();
}

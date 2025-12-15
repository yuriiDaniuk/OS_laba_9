#include "Lab9_Client.h" // (Або Lab9_Client.h)
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // --- ТУТ БУЛА ЗМІНА НАЗВИ ---
    // Ми пишемо :/StyleSheet.css, бо саме так він називається у твоєму .qrc файлі
    QFile file(":/StyleSheet.css");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    // ----------------------------

    Lab9_Client w; // (Або Lab9_Client w;)
    w.show();
    return a.exec();
}
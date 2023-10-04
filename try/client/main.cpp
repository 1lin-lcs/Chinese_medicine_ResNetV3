#include "Chinese_Medicine_Client.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Chinese_Medicine_Client w;
    w.show();
    return a.exec();
}

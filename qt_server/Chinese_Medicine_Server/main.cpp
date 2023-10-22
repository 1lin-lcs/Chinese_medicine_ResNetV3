#include <QCoreApplication>
#include "chinese_medicine_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Chinese_Medicine_Server server;
    if(!server.ReadConfig())
        return -1;
    if(server.Start())
        return -2;
    return a.exec();
}

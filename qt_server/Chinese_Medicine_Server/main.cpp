#include <QCoreApplication>
#include "chinese_medicine_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<ServerConfig>();
    qRegisterMetaType<DataBaseInfo>();
    Chinese_Medicine_Server server;
    if(!server.ReadConfig())
        return -1;
    if(!server.Start())
        return -2;
#ifdef UsePython
    if(!server.InitPython())
        return -3;
#endif
    return a.exec();
}

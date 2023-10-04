#include "mytcpServer.h"
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer* tcpServer=new MyTcpServer(nullptr);
    return a.exec();
}

#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QObject>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

protected:
    void imcomingConnection(qintptr);  //实现多线程，获得嵌套字

signals:
    void SocketDesc(qintptr socketDescriptor);
};

#endif // MYTCPSERVER_H

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
    void incomingConnection(qintptr);  //实现多线程，获得嵌套字

signals:
    void MySocketDesc(qintptr socketDescriptor);
};

#endif // MYTCPSERVER_H

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
    void imcomingConnection(qintptr socketDescriptor);  //ʵ�ֶ��̣߳����Ƕ����

signals:
    void SocketDesc(qintptr socketDescriptor);
};

#endif // MYTCPSERVER_H

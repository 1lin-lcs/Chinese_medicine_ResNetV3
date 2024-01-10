#ifndef MYTCPTASK_H
#define MYTCPTASK_H

#include <QObject>
#include <QRunnable>


class MyTcpTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MyTcpTask();
    ~MyTcpTask();
    void run() override;
    void setSocketDescriptor(qintptr);              //设置socket套接字

private:
    qintptr socketDescriptor;                       //当前socket的套接字

public slots:
    void GetJsonFile(QJsonDocument*,qintptr);       //接收的Json内容
    void GetData(QJsonDocument*,qintptr);           //发送的Json数据

signals:
    void SendJsonFile(QJsonDocument*,qintptr);      //传递接收的Json内容
    void SendData(QJsonDocument*,qintptr);          //传递发送的Json数据
};

#endif // MYTCPTASK_H

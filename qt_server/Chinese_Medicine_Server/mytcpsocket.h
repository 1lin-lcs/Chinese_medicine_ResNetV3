#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>

#ifdef ShowInformation
#include <QJsonObject>
#endif

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    MyTcpSocket(qintptr);
    qintptr GetDesc();
    ~MyTcpSocket();
private:
    QByteArray JsonData;
    QJsonParseError JsonError;
    qintptr socketDesc;

#ifdef ShowInformation
    void ShowData(QJsonDocument*);
#endif

public slots:
    void GetJsonFile();                     //处理是否收到完整的Json文件
    void SendData(QJsonDocument*,qintptr);  //发送服务器生成的数据
signals:
    void SendJsonFile(QJsonDocument*,qintptr);
};

#endif // MYTCPSOCKET_H

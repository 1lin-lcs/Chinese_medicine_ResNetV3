#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    MyTcpSocket(qintptr);
private:
    QByteArray* JsonData=new QByteArray();
    QJsonParseError JsonError;

    qintptr socketDesc;
private slots:
    void GetJsonFile();        //处理是否收到完整的Json文件
    void SendData(QJsonDocument*,qintptr);
signals:
    void SendJsonFile(QJsonDocument*,qintptr);
};

#endif // MYTCPSOCKET_H

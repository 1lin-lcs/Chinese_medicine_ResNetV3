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
private:
    QByteArray* JsonData=new QByteArray();
    QJsonParseError JsonError;
private slots:
    void GetJsonFile();        //�����Ƿ��յ�������Json�ļ�
signals:
    void SendJsonFile(QJsonDocument*);
};

#endif // MYTCPSOCKET_H

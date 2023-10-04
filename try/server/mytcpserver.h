#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "myConDB.h"
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qnetworkproxy.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>
#include <qtimer.h>
#include <qfile.h>
#include <qdir.h>
#include <qdatetime.h>
#include "IdentityThread.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private:
    void Start();
    void Myconnect();
    void task(QTcpSocket*);
    void TaskSignIn(QJsonObject,QTcpSocket*);
    void TaskSignUp(QJsonObject,QTcpSocket*);
    void TaskIdentify(QJsonObject, QTcpSocket*);
    QByteArray CreateJson(int,int,QString);
    QByteArray CreateJsonInfo(int,int, QString,QStringList);
    void ReadConfig();
    void InitPython();

    QTcpServer* server=nullptr;
    myConDB* conDB=nullptr;
    QTimer* MonitorSC=nullptr;
    int SocketCount = 0;
    int ExitCount = 0;
    bool IsAutoQuit = true;
    int AQIntervalTimeTime=0;
    int QuitCount=0;
    QByteArray SumData;
    QString HostName;
    int Port;
    QString DataBaseName;
    QString Username;
    QString Password;

private slots:
    void acceptConnection();
    void JudgementExit();
    void ReceiveEnd(QString,QTcpSocket*);
    void GetPassword(QString, QTcpSocket*);
    void ChangePassword(QJsonObject, QJsonObject,QTcpSocket*);
    void GetThreadError(QString);
};

#endif // MYTCPSERVER_H

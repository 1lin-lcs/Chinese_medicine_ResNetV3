#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Chinese_Medicine_Client.h"
#include <qfiledialog.h>
#include <QtNetwork/qtcpsocket.h>
#include <qtimer.h>
#include "Sign.h"
#include "UserInfo.h"

class Chinese_Medicine_Client : public QMainWindow
{
    Q_OBJECT

public:
    Chinese_Medicine_Client(QWidget *parent = nullptr);
    ~Chinese_Medicine_Client();

private:
    Ui::Chinese_Medicine_ClientClass ui;
    void MyConnect();
    void StartLink();
    QByteArray MakeJsonData(int,int,QString,QString,QString,QString);
    void ReadConfig();
    void WriteConfig(bool,QString, QString, QString);
    void DealLogIn();
    void IdentitfSignIn(QJsonObject);

    QString Imgfile = "";
    QTcpSocket* socket;
    QTimer* timer;
    bool LinkState = false;
    bool IsSignIn = false;
    bool IsSendSignIn = false;
    bool IsSendSignUp = false;
    QString Username;
    QString Email;
    QString Password;
   
private slots:
    void OpenPic();
    void TestStatus();
    void ReadServerData();
    void GetInfo(QStringList);
    void SignIn();
    void SignUp();
    void ShowInfo();
    void IdentifyPic();
    void SendNPassword(QString);
    void UserLogOut();
};

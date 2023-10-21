#ifndef CHINESE_MEDICINE_SERVER_H
#define CHINESE_MEDICINE_SERVER_H

#include <QObject>
#include "mydatabase.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QTimer>

//�������õĽṹ�壬��������������
struct ServerConfig {
    bool IsAutoQuit;
    int InvervalTime;
    int QuitCoint;
};

//�������˵�¼������������
//ע�⣬���struct��Ҫʹ��memset�Ƚϵͼ����ڴ��������
struct DataBaseInfo{
    QString HostName;
    int Port;
    QString DataBaseName;
    QString UserName;
    QString PassWord;
};


class Chinese_Medicine_Server
{
    Q_OBJECT
public:
    Chinese_Medicine_Server();
private:
    MyTcpServer* server=nullptr;
    MyDataBase* DataBase=nullptr;
    DataBaseInfo Info;                      //�������˵�¼���ݿ����Ϣ
    ServerConfig Serverconfig;              //�������˵�����
    QTimer Timer;                           //��ʱ���Զ��رճ���

    bool Start();                           //�������������������ݿ�
    bool ReadConfig();                      //��ȡ����
private slots:
    void CloseProgram();                    //�Զ��˳�����!!ע��!!���������������
};

#endif // CHINESE_MEDICINE_SERVER_H

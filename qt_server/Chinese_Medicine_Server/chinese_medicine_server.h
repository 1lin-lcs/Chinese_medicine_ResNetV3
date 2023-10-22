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
#include <QThread>
#include <QMap>

#define UserTable "user"

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
    DataBaseInfo Info;                          //�������˵�¼���ݿ����Ϣ
    ServerConfig Serverconfig;                  //�������˵�����
    QTimer Timer;                               //��ʱ���Զ��رճ���
    QMap<qintptr,QMap<MyTcpSocket*,QThread*>> socketMap; //����socket��thread��Ϣ

    bool Start();                               //�������������������ݿ�
    bool ReadConfig();                          //��ȡ����
    void Task(QJsonDocument*,qintptr);          //����Json�ļ�����
    void TaskSignIn(QJsonDocument*,qintptr);    //�����¼�¼�
    void TaskSignUp(QJsonDocument*,qintptr);    //����ע���¼�
    void ChangePasswd(QJsonDocument*,qintptr);  //�����޸������¼�
    void DeleteUser(QJsonDocument*,qintptr);    //ע���û��¼�
    void TaskIdentify(QJsonDocument*,qintptr);  //ͼƬʶ���¼�
    void CreateErrorJsonInfo(qintptr,QString);  //��������ʱ���ɻظ���Json�ĵ�
    void CreateSuccessJsonInfo(qintptr,int,QString);//���ش���ɹ�Json����
private slots:
    void CloseProgram();                        //�Զ��˳�����!!ע��!!���������������
    void CreateSocket();                        //�����µ�socket���������µ��߳�
    void GetJsonFile(QJsonDocument*,qintptr);   //���Json�ļ�����
    void DeleteSocketThread();                  //��socketMap�л����Ϣ��ɾ��û�����ӵ�socket��thread
signals:
    void SendJsonDoc(QJsonDocument*,qintptr);   //�����źŵ�socket����socket��������
};

#endif // CHINESE_MEDICINE_SERVER_H

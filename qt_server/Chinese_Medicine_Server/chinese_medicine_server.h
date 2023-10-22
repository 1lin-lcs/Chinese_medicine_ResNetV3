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

//保存设置的结构体，方便增加设置项
struct ServerConfig {
    bool IsAutoQuit;
    int InvervalTime;
    int QuitCoint;
};

//保存服务端登录服务器的设置
//注意，这个struct不要使用memset等较低级的内存操作函数
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
    DataBaseInfo Info;                          //保存服务端登录数据库的信息
    ServerConfig Serverconfig;                  //保存服务端的设置
    QTimer Timer;                               //计时，自动关闭程序
    QMap<qintptr,QMap<MyTcpSocket*,QThread*>> socketMap; //保存socket和thread信息

    bool Start();                               //启动监听，并连接数据库
    bool ReadConfig();                          //读取设置
    void Task(QJsonDocument*,qintptr);          //处理Json文件内容
    void TaskSignIn(QJsonDocument*,qintptr);    //处理登录事件
    void TaskSignUp(QJsonDocument*,qintptr);    //处理注册事件
    void ChangePasswd(QJsonDocument*,qintptr);  //处理修改密码事件
    void DeleteUser(QJsonDocument*,qintptr);    //注销用户事件
    void TaskIdentify(QJsonDocument*,qintptr);  //图片识别事件
    void CreateErrorJsonInfo(qintptr,QString);  //发生错误时生成回复的Json文档
    void CreateSuccessJsonInfo(qintptr,int,QString);//返回处理成功Json内容
private slots:
    void CloseProgram();                        //自动退出程序!!注意!!这个函数还不完善
    void CreateSocket();                        //创建新的socket，并移入新的线程
    void GetJsonFile(QJsonDocument*,qintptr);   //获得Json文件内容
    void DeleteSocketThread();                  //从socketMap中获得信息，删除没有连接的socket和thread
signals:
    void SendJsonDoc(QJsonDocument*,qintptr);   //发送信号到socket，让socket发送数据
};

#endif // CHINESE_MEDICINE_SERVER_H

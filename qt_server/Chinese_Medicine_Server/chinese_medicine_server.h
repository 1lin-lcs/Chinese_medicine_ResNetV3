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
    DataBaseInfo Info;                      //保存服务端登录数据库的信息
    ServerConfig Serverconfig;              //保存服务端的设置
    QTimer Timer;                           //计时，自动关闭程序

    bool Start();                           //启动监听，并连接数据库
    bool ReadConfig();                      //读取设置
private slots:
    void CloseProgram();                    //自动退出程序!!注意!!这个函数还不完善
};

#endif // CHINESE_MEDICINE_SERVER_H

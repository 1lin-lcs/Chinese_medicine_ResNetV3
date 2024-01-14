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
#include <QHash>
#include <QMetaObject>

#define UserTable "user"

#ifdef UsePython
#include "IdentityThread.h"
#endif

#ifdef UseCpp
#include "IdentityThreadC++.h"
#endif

#ifdef UseThreadPool
#include <QThreadPool>
#include "mytcptask.h"
#endif

//保存设置的结构体，方便增加设置项
struct ServerConfig {
    bool IsAutoQuit;
    int InvervalTime;
    int QuitCoint;
};
Q_DECLARE_METATYPE(ServerConfig)

//保存服务端登录服务器的设置
//注意，这个struct不要使用memset等较低级的内存操作函数
struct DataBaseInfo{
    QString HostName;
    int Port;
    QString DataBaseName;
    QString UserName;
    QString PassWord;
};
Q_DECLARE_METATYPE(DataBaseInfo)


class Chinese_Medicine_Server : public QObject
{
    Q_OBJECT
public:
    Chinese_Medicine_Server();
    bool Start();                                           //启动监听，并连接数据库
    bool ReadConfig();                                      //读取设置
#ifdef UsePython
    bool InitPython();                                      //如果使用Python作为识别，就初始化Python
#endif
private:
    MyTcpServer* server=nullptr;
    MyDataBase* DataBase=nullptr;
    DataBaseInfo Info;                                      //保存服务端登录数据库的信息
    ServerConfig Serverconfig;                              //保存服务端的设置
    QTimer Timer;                                           //计时，自动关闭程序
    int ConnectionNum=0;                                    //连接的客户端个数

#ifdef UseCpp
    QMap<qintptr,QHash<MyTcpSocket*,QThread*>> socketMap;   //保存socket和thread信息
    Module module;                                          //加载模型
    torch::DeviceType deviceType;                           //选择设备
    int categoryNum=0;                                      //植物类别数量
    QStringList categories;                                 //植物类别名称
#endif

#ifdef UseThreadPool
    QThreadPool pool;                                       //线程池
    QList<MyTcpTask*> tcpTasks;                                //线程任务
#endif

    void Task(QJsonDocument*,qintptr);                      //处理Json文件内容
    void TaskSignIn(QJsonDocument*,qintptr);                //处理登录事件
    void TaskSignUp(QJsonDocument*,qintptr);                //处理注册事件
    void ChangePasswd(QJsonDocument*,qintptr);              //处理修改密码事件
    void DeleteUser(QJsonDocument*,qintptr);                //注销用户事件
    void TaskIdentify(QJsonDocument*,qintptr);              //图片识别事件
    void CreateErrorJsonInfo(qintptr,QString);              //发生错误时生成回复的Json文档
    void CreateSuccessJsonInfo(qintptr,int,QString);        //返回处理成功Json内容

#ifdef UseCpp
    void LoadModule(QString);                               //如果使用C++，加载模型路径
    bool LoadCategories(QString);                           //如果使用C++，加载植物类别
#endif

private slots:
    void CloseProgram();                                    //自动退出程序!!注意!!这个函数还不完善

#ifdef UsePython
    void ReceiveEnd(QString,qintptr);                       //接受识别的信息
    void GetThreadError(QString);                           //接受线程错误信息
#endif

#ifdef UseCpp
    void CreateSocket(qintptr);                             //创建新的socket，并移入新的线程
    void GetJsonFile(QJsonDocument*,qintptr);               //获得Json文件内容
    void DeleteSocketThread();                              //从socketMap中获得信息，删除没有连接的socket和thread
    void ReceiverResult(QString,qintptr);                   //接受识别的信息
#endif

#ifdef UseThreadPool
    void AddSocket(qintptr);
#endif

signals:
    void SendJsonDoc(QJsonDocument*,qintptr);               //发送信号到socket，让socket发送数据
};

#endif // CHINESE_MEDICINE_SERVER_H

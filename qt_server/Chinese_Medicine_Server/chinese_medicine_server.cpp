#include "chinese_medicine_server.h"

Chinese_Medicine_Server::Chinese_Medicine_Server()
{

}

/*! @brief 这是读取配置文件用的
 *  @note 默认读取目录下的configuration.json文件
 *  @return true/false 读取成功就返回true，否则false
*/
bool Chinese_Medicine_Server::ReadConfig(){
    QFile configFile(QDir::currentPath()+"/configuration.json");
    if(!configFile.open(QIODevice::ReadOnly)){
        qDebug()<<"打开配置文件失败";
        return false;
    }
    QJsonParseError jsonerror;
    QJsonDocument doc(QJsonDocument::fromJson(configFile.readAll(),jsonerror));
    if(!doc.isNull()||jsonerror!=QJsonParseError::NoError){
        qDebug()<<"读取配置文件失败";
        return false;
    }
    QJsonObject top=doc.object();
    QJsonObject config=top.value("Config").toObject();
    QJsonObject userInfo=top.value("DataBaseUser").toObject();

    Serverconfig.IsAutoQuit=config.value("IsAutoQuit").toBool();
    Serverconfig.InvervalTime=config.value("InvervalTime").toInt();
    Serverconfig.QuitCoint=config.value("QuitCoint").toInt();

    Info.HostName=userInfo.value("HostName").toString();
    Info.Port=userInfo.value("Port").toInt();
    Info.DataBaseName=userInfo.value("DataBaseName").toString();
    Info.UserName=userInfo.value("UserName").toString();
    Info.PassWord=userInfo.value("Password").toString();
    return true;
}

/*! @brief 这是启动服务，进行监听，并连接数据库
 *  @note 确保ReadConfig()函数有运行且成功，否则出现错误
 *  @return true/false，启动成功就是true，否则false
*/
bool Chinese_Medicine_Server::Start(){
    server=new MyTcpServer();
    if(!server->listen(QHostAddress::LocalHost,50002)){
        qDebug()<<"Server启动失败";
        return false;
    }
    qDebug()<<"Server启动成功";
    DataBase=new MyDataBase(Info.HostName,Info.Port,Info.DataBaseName,Info.UserName,Info.PassWord);
    if(!DataBase->LinkDB("User")){                                               //起个名字而已，实际作用是关闭用的
        qDebug()<<"连接数据库失败";
        return false;
    }
    qDebug()<<"连接数据库成功";
    if(Serverconfig.IsAutoQuit){
        if(Serverconfig.InvervalTime>0)
            Timer.start(Serverconfig.InvervalTime*1000);
        else
            Timer.start(10*1000);
        connect(&Timer,&QTimer::timeout,this,&Chinese_Medicine_Server::CloseProgram);
    }
}

/*! @brief 自动退出程序
 *  @attention 这个函数还不完善
*/
void Chinese_Medicine_Server::CloseProgram(){
    if((ServerConfig.QuitCoint--)<0){
        if(server!=nullptr||server->isListening()){
            server->close();
            delete server;
        }
        if(DataBase!=nullptr){
            DataBase->CloseDB();
            delete DataBase;
        }
        exit(0);
    }
}

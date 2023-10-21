#include "chinese_medicine_server.h"

Chinese_Medicine_Server::Chinese_Medicine_Server()
{

}

/*! @brief ���Ƕ�ȡ�����ļ��õ�
 *  @note Ĭ�϶�ȡĿ¼�µ�configuration.json�ļ�
 *  @return true/false ��ȡ�ɹ��ͷ���true������false
*/
bool Chinese_Medicine_Server::ReadConfig(){
    QFile configFile(QDir::currentPath()+"/configuration.json");
    if(!configFile.open(QIODevice::ReadOnly)){
        qDebug()<<"�������ļ�ʧ��";
        return false;
    }
    QJsonParseError jsonerror;
    QJsonDocument doc(QJsonDocument::fromJson(configFile.readAll(),jsonerror));
    if(!doc.isNull()||jsonerror!=QJsonParseError::NoError){
        qDebug()<<"��ȡ�����ļ�ʧ��";
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

/*! @brief �����������񣬽��м��������������ݿ�
 *  @note ȷ��ReadConfig()�����������ҳɹ���������ִ���
 *  @return true/false�������ɹ�����true������false
*/
bool Chinese_Medicine_Server::Start(){
    server=new MyTcpServer();
    if(!server->listen(QHostAddress::LocalHost,50002)){
        qDebug()<<"Server����ʧ��";
        return false;
    }
    qDebug()<<"Server�����ɹ�";
    DataBase=new MyDataBase(Info.HostName,Info.Port,Info.DataBaseName,Info.UserName,Info.PassWord);
    if(!DataBase->LinkDB("User")){                                               //������ֶ��ѣ�ʵ�������ǹر��õ�
        qDebug()<<"�������ݿ�ʧ��";
        return false;
    }
    qDebug()<<"�������ݿ�ɹ�";
    if(Serverconfig.IsAutoQuit){
        if(Serverconfig.InvervalTime>0)
            Timer.start(Serverconfig.InvervalTime*1000);
        else
            Timer.start(10*1000);
        connect(&Timer,&QTimer::timeout,this,&Chinese_Medicine_Server::CloseProgram);
    }
}

/*! @brief �Զ��˳�����
 *  @attention ���������������
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

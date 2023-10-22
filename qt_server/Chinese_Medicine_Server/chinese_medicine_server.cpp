#include "chinese_medicine_server.h"

Chinese_Medicine_Server::Chinese_Medicine_Server()
{}

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
    connet(server,&MyTcpServer::SocketDesc,this,&Chinese_Medicine_Server::CreateSocket);
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

/*! @brief ���Ǵ���Json�ļ����ݵĺ���
 *  @param doc ��socket������Json�ļ���ָ��
 *  @param socketdesc socket��������
 *  @note ע�⼰ʱ���ָ�������
*/
void Chinese_Medicine_Server::Task(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject obj=doc->object();
    QJsonObject status;
    if(!obj.contains("status")){
        CreateErrorJsonInfo(socketdesc,"��Ϣȱʧ");
        delete doc;
        rerurn;
    }
    status=obj.value("status").toObject();
    if(status.value("error").toInt()!=0){
        delete doc;
        return;
    }
    switch(status.value("client").toInt()){
    case 0:TaskSignIn(doc,socketdesc);break;
    case 1:TaskSignUp(doc,socketdesc);break;
    case 2:ChangePasswd(doc,socketdesc);break;
    case 3:DeleteUser(doc,socketdesc);break;
    case 4:TaskIdentify(doc,socketdesc);break;
    default:CreateErrorJsonInfo(socketdesc,"�¼�����");break;
    }
}

/*! @brief �����¼�¼�
 *  @param doc Json��Ϣ
 *  @param socketdesc socket�������ַ�
*/
void Chinese_Medicine_Server::TaskSignIn(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"ȱʧinformation��");
        delete doc;
        return;
    }
    information=information.value("information").toObject();

    QString user=information.value("username").toString();
    QString passwd=information.value("password").toString();

    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindSingleData(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"�޴��û�");
        delete doc;
        return;
    }

    QString sql2=QString("select password from %1 where username=\"%2\"").arg(UserTable,user);
    QString res=DataBase->FindSingleData(sql2);

    if(res==""){
        CreateErrorJsonInfo(socketdesc,"û����������");
        delete doc;
        return;
    }

    if(res.compare(passwd)!=0){
        CreateErrorJsonInfo(socketdesc,"�������");
        delete doc;
        return;
    }

    CreateSuccessJsonInfo(socketdesc,0,"����˻���Ϣ�ɹ������Ե�¼");
    delete doc;
}

/*! @brief ����ע����¼�
 *  @param doc Json�ĵ�������
 *  @param socketdesc socket��������
*/
void Chinese_Medicine_Server::TaskSignUp(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"����ȱʧ");
        delete doc;
        return;
    }
    information=information.value("information").toObject();

    QString username=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,username);
    if(DataBase->FindSingleData(sql1)!=""){
        CreateErrorJsonInfo(socketdesc,"�û��Ѵ���");
        delete doc;
        return;
    }

    QString passwd=information.value("password").toString();
    QString email=information.value("email").toString();
    QString sql2=QString("insert into %1 (username,password,email,create_time) values ('%2','%3','%4',now())").arg(UserTable,username,passwd,email);
    if(DataBase->InsertData(sql2)){
        CreateSuccessJsonInfo(socketdesc,1,"ע��ɹ�");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"ע��ʧ�ܣ�����˵�����");
        delete doc;
        return;
    }
}

/*! @brief �����޸������¼�
 *  @param doc Json�ĵ�������
 *  @param socketdesc socket��������
*/
void Chinese_Medicine_Server::ChangePasswd(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"����ȱʧ");
        delete doc;
        return;
    }

    information=information.value("information").toObject();

    QString user=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindDatas(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"�û�������");
        delete doc;
        return;
    }

    QString passwd=information.value("password").toString();
    QString sql2=QString("select password from %1 where username=\"%2\"").arg(UserTable,user);
    QString res=DataBase->FindSingleData(sql2);
    if(passwd!=res){
        CreateErrorJsonInfo(socketdesc,"ԭ�������");
        delete doc;
        return;
    }
    QString sql3=QString("update %1 set password=\"%2\" where username = \"%3\"").arg(UserTable,passwd,user);
    if(DataBase->UpdataData(sql3)){
        CreateSuccessJsonInfo(socketdesc,2,"�޸ĳɹ�");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"�޸�ʧ�ܣ�����˵�����");
        delete doc;
        return;
    }
}

/*! @brief ����ע���û��¼�
 *  @param doc Json�ĵ�������
 *  @param socketdesc socket��������
*/
void Chinese_Medicine_Server::DeleteUser(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"����ȱʧ");
        delete doc;
        return;
    }

    information=information.value("information").toObject();
    QString user=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindDatas(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"�û�������");
        delete doc;
        return;
    }

    QString sql2=QString("delete from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->DeleteData(sql2)){
        CreateSuccessJsonInfo(socketdesc,3,"ע���ɹ�");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"ע��ʧ�ܣ�����˵�����");
        delete doc;
        return;
    }
}

/*! @brief ����ͼƬʶ���¼�
 *  @param doc Json�ĵ�������
 *  @param socketdesc socket��������
*/
void Chinese_Medicine_Server::TaskIdentify(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject response=doc->object();
    if(!response.contains("response")){
        CreateErrorJsonInfo(socketdesc,"����ȱʧ");
        delete doc;
        return;
    }
    response=response.value("response").toObject();
}

/*! @brief ���ɺ��д�����Ϣ��Json����
 *  @param socketdesc socket�������ַ�
 *  @param info д��response������
*/
void Chinese_Medicine_Server::CreateErrorJsonInfo(qintptr socketdesc,QString info){
    QJsonObject top,status,response;

    status.insert("error",1);

    response.insert("data",info);

    top.insert("status",status);
    top.insert("response",response);

    QJsonDocument* doc=new QJsonDocument(top);
    emit(SendJsonDoc(doc,socketdesc));
}

/*! @brief ���ɳɹ�������Json����
 *  @param socketdesc socket�������ַ�
 *  @param event �¼�����
 *  @param info д��response������
*/
void Chinese_Medicine_Server::CreateSuccessJsonInfo(qintptr socketdesc, int event, QString info){
    QJsonObject top,status,response;

    status.insert("server",event);
    status.insert("error",0);

    response.insert("data",info);

    top.insert("status",status);
    top.insert("response",response);

    QJsonDocument* doc=new QJsonDocument(top);
    emit(SendJsonDoc(doc,socketdesc));
}


//normal function
//-----------------------------------------------------------------------------------
//slot function


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

/*! @brief �����µ�socket���������̴߳���
*/
void Chinese_Medicine_Server::CreateSocket(qintptr socketdesc){
    MyTcpSocket* socket=new MyTcpSocket(socketdesc);

    connect(socket,&MyTcpSocket::SendJsonFile,this,&Chinese_Medicine_Server::GetJsonFile);
    connect(socket,&MyTcpSocket::readyRead,socket,&MyTcpSocket::GetJsonFile);
    connect(this,&Chinese_Medicine_Server::SendJsonDoc,socket,&MyTcpSocket::SendData);
    connect(socket,&MyTcpSocket::disconnected,this,&Chinese_Medicine_Server::DeleteSocketThread,Qt::QueuedConnection);

    QThread* thread=new QThread(this);
    socket->moveToThread(thread);

    QMap<MyTcpSocket*,QThread*> map(socket,thread);
    socketMap.insert(socketdesc,map);
}

/*! @brief ���ܴ�socket������Json�ļ�
 *  @param doc ��socket������Json�ļ���ָ��
 *  @param socketdesc socket��������
*/
void Chinese_Medicine_Server::GetJsonFile(QJsonDocument* doc,qintptr socketdesc){
    Task(doc,socketdesc);
}

/*! @brief ɾ��û�����ӵ�socket��thread
*/
void Chinese_Medicine_Server::DeleteSocketThread(){
    MyTcpSocket* socket=(MyTcpServer*)sender();
    qintptr handle=socket->GetDesc();
    QMap temp=socketMap.value(handle);
    QThread* thread=temp.value(socket);
    socket->deleteLater();
    thread->deleteLater();
    socketMap.remove(handle);
}

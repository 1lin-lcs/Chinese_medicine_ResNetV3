#include "chinese_medicine_server.h"

Chinese_Medicine_Server::Chinese_Medicine_Server()
{}

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
    connet(server,&MyTcpServer::SocketDesc,this,&Chinese_Medicine_Server::CreateSocket);
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

/*! @brief 这是处理Json文件内容的函数
 *  @param doc 从socket发来的Json文件的指针
 *  @param socketdesc socket的描述符
 *  @note 注意及时清除指针的内容
*/
void Chinese_Medicine_Server::Task(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject obj=doc->object();
    QJsonObject status;
    if(!obj.contains("status")){
        CreateErrorJsonInfo(socketdesc,"信息缺失");
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
    default:CreateErrorJsonInfo(socketdesc,"事件错误");break;
    }
}

/*! @brief 处理登录事件
 *  @param doc Json信息
 *  @param socketdesc socket的描述字符
*/
void Chinese_Medicine_Server::TaskSignIn(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"缺失information键");
        delete doc;
        return;
    }
    information=information.value("information").toObject();

    QString user=information.value("username").toString();
    QString passwd=information.value("password").toString();

    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindSingleData(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"无此用户");
        delete doc;
        return;
    }

    QString sql2=QString("select password from %1 where username=\"%2\"").arg(UserTable,user);
    QString res=DataBase->FindSingleData(sql2);

    if(res==""){
        CreateErrorJsonInfo(socketdesc,"没有设置密码");
        delete doc;
        return;
    }

    if(res.compare(passwd)!=0){
        CreateErrorJsonInfo(socketdesc,"密码错误");
        delete doc;
        return;
    }

    CreateSuccessJsonInfo(socketdesc,0,"获得账户信息成功，可以登录");
    delete doc;
}

/*! @brief 处理注册的事件
 *  @param doc Json文档的内容
 *  @param socketdesc socket的描述符
*/
void Chinese_Medicine_Server::TaskSignUp(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"内容缺失");
        delete doc;
        return;
    }
    information=information.value("information").toObject();

    QString username=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,username);
    if(DataBase->FindSingleData(sql1)!=""){
        CreateErrorJsonInfo(socketdesc,"用户已存在");
        delete doc;
        return;
    }

    QString passwd=information.value("password").toString();
    QString email=information.value("email").toString();
    QString sql2=QString("insert into %1 (username,password,email,create_time) values ('%2','%3','%4',now())").arg(UserTable,username,passwd,email);
    if(DataBase->InsertData(sql2)){
        CreateSuccessJsonInfo(socketdesc,1,"注册成功");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"注册失败，服务端的问题");
        delete doc;
        return;
    }
}

/*! @brief 处理修改密码事件
 *  @param doc Json文档的内容
 *  @param socketdesc socket的描述符
*/
void Chinese_Medicine_Server::ChangePasswd(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"内容缺失");
        delete doc;
        return;
    }

    information=information.value("information").toObject();

    QString user=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindDatas(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"用户不存在");
        delete doc;
        return;
    }

    QString passwd=information.value("password").toString();
    QString sql2=QString("select password from %1 where username=\"%2\"").arg(UserTable,user);
    QString res=DataBase->FindSingleData(sql2);
    if(passwd!=res){
        CreateErrorJsonInfo(socketdesc,"原密码错误");
        delete doc;
        return;
    }
    QString sql3=QString("update %1 set password=\"%2\" where username = \"%3\"").arg(UserTable,passwd,user);
    if(DataBase->UpdataData(sql3)){
        CreateSuccessJsonInfo(socketdesc,2,"修改成功");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"修改失败，服务端的问题");
        delete doc;
        return;
    }
}

/*! @brief 处理注销用户事件
 *  @param doc Json文档的内容
 *  @param socketdesc socket的描述符
*/
void Chinese_Medicine_Server::DeleteUser(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject information=doc->object();
    if(!information.contains("information")){
        CreateErrorJsonInfo(socketdesc,"内容缺失");
        delete doc;
        return;
    }

    information=information.value("information").toObject();
    QString user=information.value("username").toString();
    QString sql1=QString("select username from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->FindDatas(sql1)==""){
        CreateErrorJsonInfo(socketdesc,"用户不存在");
        delete doc;
        return;
    }

    QString sql2=QString("delete from %1 where username=\"%2\"").arg(UserTable,user);
    if(DataBase->DeleteData(sql2)){
        CreateSuccessJsonInfo(socketdesc,3,"注销成功");
        delete doc;
        return;
    }
    else{
        CreateErrorJsonInfo(socketdesc,"注销失败，服务端的问题");
        delete doc;
        return;
    }
}

/*! @brief 处理图片识别事件
 *  @param doc Json文档的内容
 *  @param socketdesc socket的描述符
*/
void Chinese_Medicine_Server::TaskIdentify(QJsonDocument* doc, qintptr socketdesc){
    QJsonObject response=doc->object();
    if(!response.contains("response")){
        CreateErrorJsonInfo(socketdesc,"内容缺失");
        delete doc;
        return;
    }
    response=response.value("response").toObject();
}

/*! @brief 生成含有错误信息的Json内容
 *  @param socketdesc socket的描述字符
 *  @param info 写入response的内容
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

/*! @brief 生成成功处理后的Json内容
 *  @param socketdesc socket的描述字符
 *  @param event 事件类型
 *  @param info 写入response的内容
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

/*! @brief 创建新的socket并移入新线程处理
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

/*! @brief 接受从socket传来的Json文件
 *  @param doc 从socket发来的Json文件的指针
 *  @param socketdesc socket的描述符
*/
void Chinese_Medicine_Server::GetJsonFile(QJsonDocument* doc,qintptr socketdesc){
    Task(doc,socketdesc);
}

/*! @brief 删除没有连接的socket和thread
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

#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject* parent)
    : QObject{ parent }
{
    Myconnect();
    ReadConfig();
    InitPython();
}

MyTcpServer::~MyTcpServer()
{

}

/**
 * @brief 连接槽函数
 */
void MyTcpServer::Myconnect() {
    
}

/**
 * @brief 初始化python
 */
void MyTcpServer::InitPython() {
    Py_Initialize();
    if (!Py_IsInitialized()) {
        qDebug() << "初始化失败";
        return;
    }
    PyEval_InitThreads();
    int nInit = PyEval_ThreadsInitialized();
    if (nInit) {
        PyEval_SaveThread();
    }
}

/**
 * @brief 用来读取配置的Json文件
 */
void MyTcpServer::ReadConfig() {
    QFile file(QDir::currentPath()+"/configuration.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件，使用默认设置";
        return;
    }
    QJsonParseError jsonerror;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jsonerror);
    if (jsonerror.error != QJsonParseError::NoError) {
        qDebug() << "读取文件出错";
        qDebug() << jsonerror.errorString();
        qDebug() << jsonerror.offset;
        return;
    }
    QJsonObject json = doc.object();
    IsAutoQuit = json.value("IsAutoQuit").toBool();
    AQIntervalTimeTime = json.value("IntervalTime").toInt();
    QuitCount = json.value("QuitCount").toInt();
    HostName = json.value("information").toObject().value("HostName").toString();
    Port = json.value("information").toObject().value("Port").toInt();
    DataBaseName = json.value("information").toObject().value("DataBaseName").toString();
    Username = json.value("information").toObject().value("Username").toString();
    Password = json.value("information").toObject().value("Password").toString();
    //qDebug() << HostName << Port << DataBaseName << Username << Password;
    Start();
}

/**
 * @brief 启动server
 */
void MyTcpServer::Start() {
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    server = new QTcpServer(this);
    qDebug() << "Server启动";
    qDebug() << "连接数据库";
    conDB = new myConDB(nullptr);
    conDB->LinkDB(HostName,Port,DataBaseName,Username,Password);
    qDebug() << "启动监听";
    //server.waitForNewConnection(100 * 1000);
    bool res = server->listen(QHostAddress::LocalHost, 50002);
    connect(server, &QTcpServer::newConnection, this, &MyTcpServer::acceptConnection);
    //qDebug() << server->serverAddress();
    if (!res) {
        qDebug() << server->errorString();
    }
    else {
        qDebug() << "没有问题";
    }
    if(IsAutoQuit)
    {
        MonitorSC = new QTimer();
        if(AQIntervalTimeTime>0)
            MonitorSC->start(AQIntervalTimeTime * 1000);
        else {
            MonitorSC->start(10 * 1000);
        }
        connect(MonitorSC, &QTimer::timeout, this, &MyTcpServer::JudgementExit);
    }
}

/**
 * @brief 这是接受新连接用的，回复新连接
 */
void MyTcpServer::acceptConnection() {
    qDebug()<< "新的连接" ;
    QByteArray data = CreateJson(0, 0, "");
    QTcpSocket* socket = server->nextPendingConnection();
    SocketCount++;
    socket->write(data,data.size());
    QByteArray array;
    connect(socket, &QTcpSocket::readyRead, this, [=] {task(socket); });
    connect(socket, &QTcpSocket::disconnected, this, [=] {SocketCount--; socket->close(); });
}

/**
 * @brief 这是处理客户端数据的
 * @param socket socket连接
 */
void MyTcpServer::task(QTcpSocket* socket) {
    bool LinkState=false;
    QByteArray data = socket->readAll();
    QJsonObject obj;
    QJsonParseError jsonerror;
    SumData.append(data);
    QJsonDocument doc(QJsonDocument::fromJson(SumData, &jsonerror));
    //qDebug() << SumData.size();
    if (jsonerror.error != QJsonParseError::NoError) {
        //qDebug() << "传输的json信息有错误";
        return;
    }
    SumData.clear();
    obj = doc.object(); 
    QJsonObject state;
    state = obj.value("state").toObject();
    switch (state.value("error").toInt()) {
    case 0: LinkState=true; break;
    case 1:qDebug() << "有问题"; break;
    default:qDebug() << "出现未知错误"; break;
    }
    if (!LinkState)
        return;
    switch (state.value("event").toInt())
    {
    case 1:TaskSignIn(obj.value("information").toObject(), socket); break;
    case 2:TaskSignUp(obj.value("information").toObject(), socket); break;
    case 5:TaskIdentify(obj.value("state").toObject(),socket); break;
    case 7:ChangePassword(obj.value("state").toObject(), obj.value("information").toObject(), socket); break;
    case 8:GetPassword(obj.value("state").toObject().value("result").toString(), socket); break;
    default:break;
    }
}

/**
 * @brief 处理登录操作
 * @param obj Json文件中的information对象
 */
void MyTcpServer::TaskSignIn(QJsonObject obj,QTcpSocket* socket) {
    qDebug() << "处理登录";
    QString username = obj.value("username").toString();
    QString password = obj.value("password").toString();
    QString sql = QString("select password from user where username=\"%1\"").arg(username);
    QString res=conDB->FindData(sql, 2);
    if (res=="" || res.compare(password) != 0) {
        QByteArray data = CreateJson(4, 0, "用户不存在");
        socket->write(data, data.size());
    }
    else {
        QString sql_more=QString("select username,password,email from user where username=\"%1\"").arg(username);
        QStringList info = conDB->FindDatas(sql_more);
        QByteArray data = CreateJsonInfo(3, 0, "登录成功",info);
        socket->write(data, data.size());
    }
}

/**
 * @brief 处理注册操作
 * @param obj Json文件中的information对象
 */
void MyTcpServer::TaskSignUp(QJsonObject obj,QTcpSocket* socket) {
    qDebug() << "处理注册";
    QString username = obj.value("username").toString();
    QString password = obj.value("password").toString();
    QString email = obj.value("email").toString();
    QString sql = QString("select username from user where username=\"%1\"").arg(username);
    QString res = conDB->FindData(sql, 1);
    if (res == "") {
        conDB->InsertData(username, password, email);
        QByteArray data = CreateJson(3, 0, "注册成功");
        socket->write(data, data.size());
    }
    else {
        QByteArray data=CreateJson(4, 0,"用户已存在");
        socket->write(data, data.size());
    }
}

/**
 * @brief 这是返回Json信息
 * @param event Json的event字段
 * @param error Json的error字段
 * @param result Json的result字段
 * @return 返回Json信息的QByteArray
 */
QByteArray MyTcpServer::CreateJson(int event,int error,QString result) {
    QJsonObject top;
    QJsonObject state;
    QJsonDocument doc;
    state.insert("event", event);
    state.insert("error", error);
    state.insert("result", result);
    top.insert("state", state);
    doc.setObject(top);
    QByteArray data;
    data = doc.toJson();
    return data;
}

/**
 * @brief 这是返回完整的Json信息
 * @param event Json的event字段
 * @param error Json的error字段
 * @param result Json的result字段
 * @param infos Json的剩余信息
 */
QByteArray MyTcpServer::CreateJsonInfo(int event, int error, QString result, QStringList infos) {
    QJsonObject top;
    QJsonObject state;
    QJsonObject information;
    QJsonDocument doc;
    state.insert("event", event);
    state.insert("error", error);
    state.insert("result", result);
    information.insert("username", infos.at(0));
    information.insert("password", infos.at(1));
    information.insert("email", infos.at(2));
    top.insert("state", state);
    top.insert("information", information);
    doc.setObject(top);
    QByteArray data;
    data = doc.toJson();
    return data;
}

/**
 * @brief 判断是否关闭程序
 */
void MyTcpServer::JudgementExit() {
    if (SocketCount > 0) {
        return;
    }
    QuitCount = (QuitCount > 0) ? QuitCount : 3;
    if (ExitCount >= QuitCount) {
        if (MonitorSC!=nullptr && MonitorSC->isActive()){
            MonitorSC->stop();
            delete MonitorSC;
        }
        if (server!=nullptr && server->isListening()) {
            server->close();
            delete server;
        }
        QDir path(QDir::currentPath() + "/tmp");
        path.removeRecursively();
        PyGILState_Ensure();
        Py_Finalize();
        qDebug() << "长时间没有连接，程序退出";
        exit(0);
    }
    else {
        ExitCount++;
    }
}

/**
 * @brief 检测图片中中草药的类型
 */
void MyTcpServer::TaskIdentify(QJsonObject obj, QTcpSocket* socket) {
    QByteArray img = QByteArray::fromBase64(obj.value("result").toVariant().toByteArray());
    QString filetype = obj.value("filetype").toString();
    QString path = QDir::currentPath() + "/tmp/" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") + filetype;
    QDir dir;
    if (!dir.exists(QDir::currentPath() +"/tmp")) {
        dir.mkdir(QDir::currentPath() +"./tmp");
    }
    QFile file(path);
    //qDebug() << path << filetype;
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "写入文件失败";
        return;
    }
    file.write(img);
    file.close();
    IdentityThread* thread = new IdentityThread(path,QDir::currentPath(),socket);
    thread->start();
    connect(thread, &IdentityThread::SendEnd,this, &MyTcpServer::ReceiveEnd);
    connect(thread, &IdentityThread::SendError, this, &MyTcpServer::GetThreadError);
    //connect(thread, &IdentityThread::finished, [=] {if (thread->isFinished())delete thread; else thread->requestInterruption(); });
}

/**
 * @brief 发送结果
 * @param end 识别结果
 */
void MyTcpServer::ReceiveEnd(QString end,QTcpSocket* socket) {
    QByteArray data=CreateJson(6,0,end);
    socket->write(data,data.size());
}

/**
 * @brief 查询密码
 */
void MyTcpServer::GetPassword(QString username,QTcpSocket* socket) {
    QString sql = QString("select password from user where username=\"%1\"").arg(username);
    QString res = conDB->FindData(sql, 2);
    QByteArray data = CreateJson(8, 0, res);
    socket->write(data, data.size());
}

/**
 * @brief 修改密码
 */
void MyTcpServer::ChangePassword(QJsonObject username, QJsonObject password,QTcpSocket* socket) {
    QString user = username.value("username").toString();
    QString pw = password.value("result").toString();
    QString sql = QString("update user set \"%1\" where username = \"%2\"").arg(pw,user);
    bool ok=conDB->UpdataData(sql);
    int state;
    if (ok) {
        state = 3;
    }
    else {
        state = 4;
    }
    QByteArray data = CreateJson(state,0,"");
    socket->write(data, data.size());
}

/**
 * @brief 获得线程的错误输出
 */
void MyTcpServer::GetThreadError(QString err) {
    qDebug() << err;
}
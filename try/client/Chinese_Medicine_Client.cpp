#include "Chinese_Medicine_Client.h"

Chinese_Medicine_Client::Chinese_Medicine_Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    MyConnect();
    ReadConfig();
    DealLogIn();
    StartLink();
}

Chinese_Medicine_Client::~Chinese_Medicine_Client()
{}

/**
 * @brief 注册槽函数
 */
void Chinese_Medicine_Client::MyConnect(){
    connect(ui.ChooseButton, &QPushButton::clicked, this, &Chinese_Medicine_Client::OpenPic);
    connect(ui.actionOpenPic, &QAction::triggered, this, &Chinese_Medicine_Client::OpenPic);
    connect(ui.actionSignUp, &QAction::triggered, this, &Chinese_Medicine_Client::SignUp);
    connect(ui.actionSignIn, &QAction::triggered, this, &Chinese_Medicine_Client::SignIn);
    connect(ui.IdentifyButton, &QPushButton::clicked, this, &Chinese_Medicine_Client::IdentifyPic);
}

/**
 * @brief 选择图片打开
 */
void Chinese_Medicine_Client::OpenPic(){
    Imgfile = QFileDialog::getOpenFileName(this, "打开图片", QDir::currentPath(), "Image(*.jpg *.png *.jpeg)");
    if (Imgfile == "") {
        return;
    }
    QPixmap pix(Imgfile);
    QPixmap dest = pix.scaled(ui.PicLabel->size(), Qt::KeepAspectRatio);
    ui.PicLabel->setPixmap(dest);
}

/**
 * @brief 连接服务器
 */
void Chinese_Medicine_Client::StartLink() {
    qDebug() << "开启连接";
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 50002);
    connect(socket, &QTcpSocket::connected, this, [this] {qDebug() << "连接成功"; });
    connect(socket, &QTcpSocket::readyRead, this, &Chinese_Medicine_Client::ReadServerData);
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Chinese_Medicine_Client::TestStatus);
    timer->start(5 * 1000);
}

/**
 * @brief 测试连接状态
 */
void Chinese_Medicine_Client::TestStatus() {
    switch (socket->state())
    {
    case QAbstractSocket::UnconnectedState:qDebug() << "未连接状态"; break;
    case QAbstractSocket::HostLookupState:qDebug() << "正在解析主机名"; break;
    case QAbstractSocket::ConnectingState:qDebug() << "正在建立连接"; break;
    case QAbstractSocket::ConnectedState:qDebug() << "已连接状态"; break;
    case QAbstractSocket::BoundState:qDebug() << "已绑定状态"; break;
    case QAbstractSocket::ClosingState:qDebug() << "正在关闭连接"; break;
    case QAbstractSocket::ListeningState:qDebug() << "正在监听连接"; break;
    default:break;
    }
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        socket->connectToHost(QHostAddress::LocalHost, 50002);
    }
    else
        timer->stop();
}

/**
 * @brief 这是用来解析服务器发送的数据
 */
void Chinese_Medicine_Client::ReadServerData() {
    QByteArray data = socket->readAll();
    QJsonObject obj;
    QJsonParseError jsonerror;
    QJsonDocument doc(QJsonDocument::fromJson(data, &jsonerror));
    if (jsonerror.error != QJsonParseError::NoError) {
        qDebug() << "传输的json信息有错误";
        return;
    }
    obj = doc.object();
    QJsonObject state;
    state = obj.value("state").toObject();
    switch (state.value("error").toInt()) {
    case 0:qDebug() << "没有问题"; LinkState = true; break;
    case 1:qDebug() << "有问题"; break;
    default:qDebug() << "出现未知错误"; break;
    }
    if (!LinkState) {
        return;
    }
    //qDebug() <<"event是" << state.value("event").toInt();
    switch (state.value("event").toInt())
    {
    case 3:QMessageBox::information(this, "提示", state.value("result").toString()); IdentitfSignIn(obj.value("information").toObject()); break;
    case 4:QMessageBox::information(this, "错误", state.value("result").toString()); break;
    case 6:ui.NameLabel->setText(state.value("result").toString());QMessageBox::information(this,"提示","识别成功"); break;
    default:
        break;
    }
}

/**
 * @brief 这是登录用的函数
 */
void Chinese_Medicine_Client::SignIn() {
    Sign* sign = new Sign(nullptr, 0);
    sign->setAttribute(Qt::WA_DeleteOnClose);
    sign->show();
    connect(sign, &Sign::SendInfo, this, &Chinese_Medicine_Client::GetInfo);
    IsSendSignIn = true;
}

/**
 * @brief 这是注册用的函数
 */
void Chinese_Medicine_Client::SignUp() {
    Sign* sign = new Sign(nullptr, 1);
    sign->setAttribute(Qt::WA_DeleteOnClose);
    sign->show();
    connect(sign, &Sign::SendInfo, this, &Chinese_Medicine_Client::GetInfo);
    IsSendSignUp = true;
}

/**
 * @brief 这是用来获得登录/注册界面信息的函数
 * @param info 登录/注册的信息
 */
void Chinese_Medicine_Client::GetInfo(QStringList info){
    if (info.at(0).compare("0") == 0) {
        QByteArray data = MakeJsonData(1, 0, "", info.at(1), info.at(2),"");
        socket->write(data, data.size());
    }
    else if (info.at(0).compare("1") == 0) {
        QByteArray data = MakeJsonData(2, 0, "", info.at(1), info.at(2),info.at(3));
        Username = info.at(1);
        Password = info.at(2);
        Email = info.at(3);
        socket->write(data, data.size());
        //qDebug() << Username << Password << Email;
    }
}

/**
 * @brief ByteArray生成json文件信息
 * @param event 事件类型
 * @param error 是否出错
 * @param result 返回结果
 * @param username 用户信息
 * @param password 用户密码
 * @param email 用户邮箱
 */
QByteArray Chinese_Medicine_Client::MakeJsonData(int event, int error, QString result, QString username, QString password, QString email) {
    QJsonDocument doc;
    QJsonObject top;
    QJsonObject state;
    QJsonObject information;
    state.insert("event", event);
    state.insert("error", error);
    state.insert("result", result);
    information.insert("username", username);
    information.insert("password", password);
    information.insert("email", email);
    top.insert("state", state);
    top.insert("information", information);
    doc.setObject(top);
    QByteArray data;
    data = doc.toJson();
    return data;
}

void Chinese_Medicine_Client::IdentifyPic() {
    QJsonDocument doc;
    QJsonObject top;
    QJsonObject state;
    state.insert("event", 5);
    state.insert("error", 0);
    QFile file(Imgfile);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "错误", "图片读取错误");
        return;
    }
    QByteArray img = file.readAll().toBase64();
    state.insert("result", QJsonValue::fromVariant(img));
    if (Imgfile.contains(".jpg") || Imgfile.contains(".jpeg")) {
        state.insert("filetype", ".jpg");
    }
    else if (Imgfile.contains(".png")) {
        state.insert("filetype", ".png");
    }
    top.insert("state", state);
    doc.setObject(top);
    QByteArray data = doc.toJson();
    socket->write(data, data.size());
    socket->waitForBytesWritten(2*1000);
}

/**
 * @brief 这是用来读取配置的
 */
void Chinese_Medicine_Client::ReadConfig(){
    QFile file("./configuration.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件，使用默认设置";
        return;
    }
    QJsonParseError jsonerror;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jsonerror);
    if (jsonerror.error != QJsonParseError::NoError) {
        qDebug() << "读取文件出错";
        qDebug() << jsonerror.errorString();
        return;
    }
    QJsonObject json = doc.object();
    IsSignIn = json.value("IsSignIn").toBool();
    QJsonObject info = json.value("Information").toObject();
    Username = info.value("Username").toString();
    Password = info.value("Password").toString();
    Email = info.value("Email").toString();
    //qDebug() << Username << Email;
    file.close();
}

/**
 * @brief 这是用来写入配置的
 */
void Chinese_Medicine_Client::WriteConfig(bool signin,QString username,QString password,QString email) {
    QFile file("./configuration.json");
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "无法打开文件，使用默认设置";
        return;
    }
    QJsonDocument doc;
    QJsonObject top;
    QJsonObject info;
    info.insert("Username", username);
    info.insert("Password", password);
    info.insert("Email", email);
    top.insert("IsSignIn", signin);
    top.insert("Information", info);
    doc.setObject(top);
    file.write(doc.toJson());
    file.close();
}

/**
 * @brief 这是处理有登录信息用的
 */
void Chinese_Medicine_Client::DealLogIn() {
    if (!IsSignIn) {
        return;
    }
    ui.actionSignIn->setText("登录信息");
    if(ui.actionSignUp->isEnabled())
        ui.actionSignUp->setDisabled(true);
    disconnect(ui.actionSignIn, &QAction::triggered, this, &Chinese_Medicine_Client::SignIn);
    connect(ui.actionSignIn, &QAction::triggered, this, &Chinese_Medicine_Client::ShowInfo);
}

/**
 * @brief 展示用户信息
 */
void Chinese_Medicine_Client::ShowInfo() {
    UserInfo* info = new UserInfo(nullptr,Username,Password,Email);
    info->setAttribute(Qt::WA_DeleteOnClose);
    info->show();
    connect(info, &UserInfo::SendPa, this, &Chinese_Medicine_Client::SendNPassword);
    connect(info, &UserInfo::UserLogout, this, &Chinese_Medicine_Client::UserLogOut);
}

/**
 * @brief 发送新的密码
 * @param 新的密码
 */
void Chinese_Medicine_Client::SendNPassword(QString password) {
    //qDebug() << password;
    IsChangePw = true;
    ChPw = password;
    QByteArray data = MakeJsonData(7, 0, password, Username, "", "");
    socket->write(data, data.size());
}

/**
 * @brief 用户退出处理
 */
void Chinese_Medicine_Client::UserLogOut() {
    IsSignIn = false;
    WriteConfig(false, "", "", "");
    ui.actionSignIn->setText("登录");
    if (!ui.actionSignUp->isEnabled())
        ui.actionSignUp->setEnabled(true);
    disconnect(ui.actionSignIn, &QAction::triggered, this, &Chinese_Medicine_Client::ShowInfo);
    connect(ui.actionSignIn, &QAction::triggered, this, &Chinese_Medicine_Client::SignIn);
}

/**
 * @brief 判断填写信息后是否登录
 */
void Chinese_Medicine_Client::IdentitfSignIn(QJsonObject result) {
    if (IsChangePw) {
        Password = ChPw;
        WriteConfig(true, Username, Password, Email);
        IsChangePw = false;
        return;
    }
    if (!IsSendSignIn && !IsSendSignUp)
        return;
    if (IsSendSignUp) {
        IsSignIn = true;
        DealLogIn();
        WriteConfig(true, Username, Password, Email);
        return;
    }
    IsSignIn = true;
    IsSendSignIn = false;
    DealLogIn();
    Username = result.value("username").toString();
    Password = result.value("password").toString();
    Email = result.value("email").toString();
    WriteConfig(true, Username, Password, Email);
    //qDebug() << Username << Email;
}
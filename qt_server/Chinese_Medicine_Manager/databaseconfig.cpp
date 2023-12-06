#include "databaseconfig.h"
#include "ui_databaseconfig.h"

DataBaseConfig::DataBaseConfig(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DataBaseConfig)
{
    reg=QRegularExpression("[a-zA-Z0-9!#@$%^&*,.;:'\"/+-<>\?~`=_]+$");
    regvalidor=new QRegularExpressionValidator(reg);
    ui->setupUi(this);
    ui->dbpassedEdit->setValidator(regvalidor);                         //设置正则表达式验证输入是否合法
    ui->dbpassedEdit->setAttribute(Qt::WA_InputMethodEnabled,false);    //设置不可以使用中文输入法
    myConnect();
}

DataBaseConfig::~DataBaseConfig()
{
    delete ui;
}

/*!
 * \brief 连接信号和槽函数
 */
void DataBaseConfig::myConnect(){
    connect(ui->showPasswd,&QCheckBox::toggled,this,&DataBaseConfig::showPasswd_toggled);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&DataBaseConfig::buttonGroup_accepted);
}

/*!
 * \brief 写入配置文件
 * \return 写入成功返回true，否则false
 */
bool DataBaseConfig::writeConfig(){
    QFile configFile(QDir::currentPath()+"/configure.json");
    if(!configFile.open(QIODevice::ReadWrite))
        return false;
    QJsonDocument doc;
    QJsonObject top;
    top.insert("HostName",ui->dbAddressEdit->text());
    top.insert("Port",ui->dbPortEdit->text());
    top.insert("DataBaseName",ui->dbNameEdit->text());
    top.insert("UserName",ui->dbuserEdit->text());
    top.insert("Password",ui->dbpassedEdit->text());
    doc.setObject(top);
    configFile.write(doc.toJson(QJsonDocument::Indented));
    configFile.close();
    return true;
}

/*!
 * \brief 改变Password显示状态
 * \param checked 是否选中
 */
void DataBaseConfig::showPasswd_toggled(bool checked){
    if(checked)
        ui->dbpassedEdit->setEchoMode(QLineEdit::Normal);
    else
        ui->dbpassedEdit->setEchoMode(QLineEdit::Password);
}

/*!
 * \brief 按下确定时写入配置文件
 */
void DataBaseConfig::buttonGroup_accepted(){
    if(!writeConfig())
        QMessageBox::warning(this,"警告","写入配置文件失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    DatabaseInfo dbInfo;
    dbInfo.hostname=ui->dbAddressEdit->text();
    dbInfo.port=ui->dbPortEdit->text().toInt();
    dbInfo.databaseName=ui->dbNameEdit->text();
    dbInfo.username=ui->dbuserEdit->text();
    dbInfo.password=ui->dbpassedEdit->text();
    emit sendConfig(dbInfo);
}

void DataBaseConfig::setInfo(DatabaseInfo* info){
    ui->dbAddressEdit->setText(info->hostname);
    ui->dbPortEdit->setText(QString::number(info->port)=="0" ? "":QString::number(info->port));
    ui->dbNameEdit->setText(info->databaseName);
    ui->dbuserEdit->setText(info->username);
    ui->dbpassedEdit->setText(info->password);
}

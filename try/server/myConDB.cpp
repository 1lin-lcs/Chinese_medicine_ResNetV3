#include "myConDB.h"

myConDB::myConDB(QObject *parent)
	: QObject(parent)
{}

myConDB::~myConDB()
{}

/**
 * @brief 连接槽函数
 * 
 */
void myConDB::Myconnect(){

}

/**
 * @brief 连接数据库
 * @param hostname 数据库的地址
 * @param port 数据库端口
 * @param databasename 数据库的名字
 * @param username 数据库用户名
 * @param password 数据库用户密码
 */
void myConDB::LinkDB(QString hostName, int port, QString databasename, QString username, QString password){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(databasename);
    db.setUserName(username);
    db.setPassword(password);
    bool ok = db.open();
    if (!ok) {
        qDebug() << db.lastError().text();
        return;
    }
    qDebug() << "连接数据库成功";
}

/**
 * @brief 插入数据到数据表
 * @param username 用户名
 * @param password 密码
 * @param email 邮箱
 */
void myConDB::InsertData(QString username, QString password, QString email){
    if (!db.isOpen()) {
        qDebug() << "数据库未连接";
    }
    query = new QSqlQuery();
    QString sqlText = QString("insert into user (username,password,email,create_time) values ('%1','%2','%3',now())").arg(username,password,email);
    if (!query->exec(sqlText)) {
        qDebug() << "插入数据失败";
    }
    query->clear();
    delete query;
}

/**
 * @brief 查找数据，只能返回一个值
 * @param sql 执行sql的语句
 * @param func 查找的功能，1是username，2是password，3是email
 * @return 查询的结果
 */
QString myConDB::FindData(QString sql,int func) {
    bool ok=false;
    query = new QSqlQuery();
    switch (func)
    {
    case 1:ok=(query->exec(sql)); break;
    case 2:ok=(query->exec(sql)); break;
    case 3:ok=(query->exec(sql)); break;
    default:
        break;
    }
    if (!ok) {
        qDebug() << "查询出错";
        return "";
    }
    if(query->next()){
        QString res = query->value(0).toString();
        query->clear();
        delete query;
        return res;
    }
    else {
        query->clear();
        delete query;
        return "";
    }
}

/**
 * @brief 更新数据
 * @return 是否更新成功
 */
bool myConDB::UpdataData(QString sql) {
    query = new QSqlQuery();
    bool ok;
    ok=query->exec(sql);
    query->clear();
    delete query;
    return ok;
}

/**
 * @brief 查找用户所有信息
 * @return 返回查找到的第一行所有值
 */
QStringList myConDB::FindDatas(QString sql) {
    query = new QSqlQuery();
    query->exec(sql);
    QSqlRecord rec = query->record();
    int count = rec.count();
    QStringList values;
    query->next();
    for (int i = 0; i < count; i++) {
        values.append(query->value(i).toString());
    }
    query->clear();
    delete query;
    return values;
}
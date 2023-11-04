#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>
#include <QStringList>

class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QObject *parent = nullptr);
    MyDataBase(QString,int,QString,QString,QString);
    ~MyDataBase();

    bool LinkDB(QString);                           //连接到数据库
    bool InsertData(QString);                       //插入数据
    QString FindSingleData(QString);                //查找单个数据
    QList<QStringList>* FindDatas(QString);         //查找多个数据
    bool UpdataData(QString);                       //更新数据
    bool DeleteData(QString);                       //删除数据
    QStringList* GetDataTableName();                //返回数据库所有表的名称
    QList<QStringList>* GetTableColumnName(QString);//返回列的名称
    bool CloseDB();                                 //关闭数据库

private:
    QString HostName="";
    int Port=0;
    QString DataBaseName="";
    QString UserName="";
    QString PassWord="";
    QSqlDatabase db;

};

#endif // MYDATABASE_H

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
    bool LinkDB(QString);                           //���ӵ����ݿ�
    bool InsertData(QString);                       //��������
    QString FindSingleData(QString);                //���ҵ�������
    QList<QStringList>* FindDatas(QString);         //���Ҷ������
    bool UpdataData(QString);                       //��������
    bool DeleteData(QString);                       //ɾ������
    QStringList* GetDataTableName();                //�������ݿ����б������
    QList<QStringList>* GetTableColumnName(QString);//�����е�����
    bool CloseDB();                                 //�ر����ݿ�

private:
    QString DataBaseName="";
    QString HostName="";
    int Port=0;
    QString DataBaseName="";
    QString UserName="";
    QString PassWord="";
    QSqlDatabase db;

};

#endif // MYDATABASE_H

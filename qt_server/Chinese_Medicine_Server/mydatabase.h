#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

class MyDataBase : public QObject
{
    Q_OBJECT
public:
    explicit MyDataBase(QObject *parent = nullptr);
    MyDataBase(QString,int,QString,QString,QString);
    bool LinkDB();                                  //���ӵ����ݿ�
    bool InsertData(QString);                       //��������
    QString FindSingleData(QString);                //���ҵ�������
    QList<QStringList>* FindDatas(QString);         //���Ҷ������
    bool UpdataData(QString);                       //��������
    bool DeleteData(QString);                       //ɾ������
    QStringList* GetDataTableName();                //�������ݿ����б������

private:
    QString HostName;
    int Port;
    QString DataBaseName;
    QString UserName;
    QString PassWord;
    QSqlDatabase db;

private slots:

};

#endif // MYDATABASE_H

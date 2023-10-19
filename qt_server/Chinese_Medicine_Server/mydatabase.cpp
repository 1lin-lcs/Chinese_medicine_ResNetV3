#include "mydatabase.h"

MyDataBase::MyDataBase(QObject *parent)
    : QObject{parent}
{

}

/** @brief ����������ʼ��Ҫ�������ݿ����Ϣ
* @param hostname ���ݿ����ڵ�ַ
* @param port ���ݿ�˿�
* @param databaseName ���ӵ����ݿ������
* @param username ��¼�û�������
* @param password ��¼�û�������
*/
MyDataBase::MyDataBase(QString hostname,int port,QString databaseName,QString username,QString password){
    HostName=hostname;
    Port=port;
    DataBaseName=databaseName;
    UserName=username;
    PassWord=password;
}

/** @brief �������ݿ�
 *  @return true/false ���ӳɹ�����true������false
*/
bool MyDataBase::LinkDB(){
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HostName);
    db.setPort(Port);
    db.setDatabaseName(DataBaseName);
    db.setUserName(UserName);
    db.setPassword(PassWord);
    bool ok=db.open();
    if(!ok){
        qDebug()<<"�����ݿ�ʧ��";
        return false;
    }
    qDebug()<<"�������ݿ�ɹ�";
    return true;
}

/** @brief �������ݵ����ݿ�
* @param sqlcommand ִ�в����������ݣ���������ɵ���������
* @return true/false ����ɹ�����true������false
*/
bool MyDataBase::InsertData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery* query=new QSqlQuery();
    if(!query->exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        delete query;
        return false;
    }
    delete query;
    return true;
}

/** @brief �������ݣ�ֻ�ܲ鵽��������
 *  @param sqlcommand ִ�в����������ݣ���������ɵ���������
 *  @return data ���ҵ��򷵻����ݣ����򷵻ؿ��ַ���
*/
QString MyDataBase::FindSingleData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return "";
    }
    QSqlQuery* query=new QSqlQuery();
    if(!query->exec(sqlcommand)){
        qDebug()<<"�鵽����ʧ��";
        delete query;
        return "";
    }
    query->next();
    QString data=query->value(0).toString();
    delete query;
    return data;
}

/** @brief �������ݣ��������в鵽������
 *  @param  sqlcommand ִ�в����������ݣ���������ɵ���������
 *  @return data �������ݵ��б��ָ�룬ʧ���򷵻ؿ�ָ��
*/
QList<QStringList>* MyDataBase::FindDatas(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return nullptr;
    }
    QSqlQuery* query=new QSqlQuery();
    if(!query->exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        delete query;
        return nullptr;
    }
    QSqlRecord record=query->record();
    int count=record.count();
    QStringList values;
    QList<QStringList> data=new QList<QStringL>;
    while(query->next()){
        for(int i=0;i<count;i++){
            values.append(query->value(i).toString());
        }
        data.append(values);
    }
    return data;
}

/** @brief ��������
 *  @param sqlcommand ִ�и����������ݣ���������ɵ���������
 *  @return true/false ���³ɹ�����true������false
*/
bool MyDataBase::UpdataData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery* query=new QSqlQuery();
    if(!query->exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        delte query;
        return false;
    }
    delete query;
    return true;
}

/** @brief ɾ�����ݣ�ע������ɾ��һ�����ݵ�
 *  @param sqlcommand ִ��ɾ���������ݣ���������ɵ���������
 *  @return true/false ɾ���ɹ�����true������false
*/
bool MyDataBase::DeleteData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery* query=new QSqlQuery;
    if(!query->exec(sqlcommand)){
        qDebug()<<"ɾ������ʧ��";
        delete query;
        return false;
    }
    delete query;
    return true;
}

/** @brief �������������ݿ����б������
 *  @return names ���б������
*/
QStringList* MyDataBase::GetDataTableName(){
    QString sqlcommand=QString("show tables");
    QSqlQuery* query=new QSqlQuery();
    if(!query->exec(sqlcommand)){
        return nullptr;
    }
    QStringList names=new QStringList;
    while(query->next()){
        names.append(query->value(0).toString());
    }
    return names;
}

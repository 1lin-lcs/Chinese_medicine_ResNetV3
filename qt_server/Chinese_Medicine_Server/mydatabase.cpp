#include "mydatabase.h"

MyDataBase::MyDataBase(QObject *parent)
    : QObject{parent}
{}

MyDataBase::~MyDataBase(){
    if(DataBaseName!="")
        QSqlDatabase::removeDatabase(DataBaseName);
}

/*! @brief ����������ʼ��Ҫ�������ݿ����Ϣ
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

/*! @brief �������ݿ�
 *  @param name �����������ݿ������
 *  @return true/false ���ӳɹ�����true������false
*/
bool MyDataBase::LinkDB(QString name){
    db=QSqlDatabase::addDatabase("QMYSQL",name);
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

/*! @brief �������ݵ����ݿ�
* @param sqlcommand ִ�в����������ݣ���������ɵ���������
* @return true/false ����ɹ�����true������false
*/
bool MyDataBase::InsertData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        return false;
    }
    return true;
}

/*! @brief �������ݣ�ֻ�ܲ鵽��������
 *  @param sqlcommand ִ�в����������ݣ���������ɵ���������
 *  @return data ���ҵ��򷵻����ݣ����򷵻ؿ��ַ���
*/
QString MyDataBase::FindSingleData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return "";
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"�鵽����ʧ��";
        return "";
    }
    query.next();
    QString data=query.value(0).toString();
    return data;
}

/*! @brief �������ݣ��������в鵽������
 *  @param  sqlcommand ִ�в����������ݣ���������ɵ���������
 *  @return data �������ݵ��б��ָ�룬ʧ���򷵻ؿ�ָ��
*/
QList<QStringList>* MyDataBase::FindDatas(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return nullptr;
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        return nullptr;
    }
    QSqlRecord record=query.record();
    int count=record.count();
    QStringList values;
    QList<QStringList> data=new QList<QStringL>;
    while(query.next()){
        for(int i=0;i<count;i++){
            if(query.value(i).isValid())
                values.append(query.value(i).toString());
            else
                values.append("");
        }
        data.append(values);
    }
    return data;
}

/*! @brief ��������
 *  @param sqlcommand ִ�и����������ݣ���������ɵ���������
 *  @return true/false ���³ɹ�����true������false
*/
bool MyDataBase::UpdataData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"��������ʧ��";
        return false;
    }
    return true;
}

/*! @brief ɾ�����ݣ�ע������ɾ��һ�����ݵ�
 *  @param sqlcommand ִ��ɾ���������ݣ���������ɵ���������
 *  @return true/false ɾ���ɹ�����true������false
*/
bool MyDataBase::DeleteData(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return false;
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"ɾ������ʧ��";
        return false;
    }
    return true;
}

/*! @brief �������������ݿ����б������
 *  @return names ���б������
*/
QStringList* MyDataBase::GetDataTableName(){
    QString sqlcommand=QString("show tables");
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        return nullptr;
    }
    QStringList names=new QStringList;
    while(query.next()){
        names.append(query.value(0).toString());
    }
    return names;
}

/*! @brief �������ݱ���ֶ���Ϣ
 *  @param sqlcommand ִ�в�ѯ���ݱ����䣬�ɵ���������
 *  @return infos ���ݱ���ֶ���Ϣ��ʧ�ܷ��ؿ�
*/
QList<QStringList>* MyDataBase::GetTableColumnName(QString sqlcommand){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return nullptr;
    }
    QSqlQuery query(sqlcommand,db);
    if(!query.exec(sqlcommand)){
        qDebug()<<"��ѯʧ��";
        return nullptr;
    }
    QSqlRecord record=query.record();
    int count=record.count();
    QStringList info;
    QList<QStringList> infos=new QList<QStringList>();
    while(query.next()){
        for(int i=0;i<count;i++){
            if(query.value(i).isValid())
                info.append(query.value(i).toString());        //������ʱ��Field|Type|Null|Key|Default|Extra�⼸������
            else
                info.append("");
        }
        infos.append(info);
    }
    return infos;
}

/*! @brief �ر����ݿ�
 *  @return true/false �Ƿ�رճɹ���Ӧ�ò������ʲô�����
*/
bool MyDataBase::CloseDB(){
    if(!db.open()){
        qDebug()<<"���ݿ�δ��";
        return true;
    }
    db.close();
    return true;
}

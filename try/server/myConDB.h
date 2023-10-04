#pragma once

#include <QObject>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h> 
#include <QtSql/qsqlrecord.h>

class myConDB  : public QObject
{
	Q_OBJECT

public:
	myConDB(QObject *parent);
	~myConDB();
	void LinkDB(QString, int, QString,QString, QString);
	void InsertData(QString, QString, QString);
	QString FindData(QString,int);
	QStringList FindDatas(QString);
	bool UpdataData(QString);

private:
	QSqlDatabase db;
	QSqlQuery* query = nullptr;
	void Myconnect();
};

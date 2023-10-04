#pragma once

#include <QWidget>
#include "ui_ChangePawd.h"
#include <qmessagebox.h>
#include <QJsonObject>
#include <qjsondocument.h>
#include <qjsonvalue.h>
#include <QByteArray>

class ChangePawd  : public QWidget
{
	Q_OBJECT

public:
	ChangePawd(QWidget *parent,QString,QString);
	~ChangePawd();

private:
	void MyConnect();

	Ui::CPawd ui;
	QString Oldpassword="";
	QString Username;
	bool IsQiut = true;

private slots:
	void ChangePd();

signals:
	void SendNP(QString);
};

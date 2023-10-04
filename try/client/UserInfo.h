#pragma once

#include <QtWidgets/QWidget>
#include "ui_UserInfo.h"
#include "ChangePawd.h"

class UserInfo  : public QWidget
{
	Q_OBJECT

public:
	UserInfo(QWidget*parent,QString,QString,QString);
	~UserInfo();

private:
	void MyConnect();

	Ui::UserInfoWidget ui;
	QString Password;
	QString Username;
	QString Email;

private slots:
	void CPassword();
	void SendPassword(QString);

signals:
	void SendPa(QString);
	void UserLogout();
};

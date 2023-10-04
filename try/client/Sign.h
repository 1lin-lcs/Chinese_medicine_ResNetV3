#pragma once

#include <QtWidgets/QWidget>
#include "ui_Sign.h"

class Sign  : public QWidget
{
	Q_OBJECT

public:
	Sign(QWidget *parent,int);
	~Sign();
private:
	Ui::Sign ui;
	void MyConnect();

	QStringList info;

private slots:
	void Accept();

signals:
	void SendInfo(QStringList);
};

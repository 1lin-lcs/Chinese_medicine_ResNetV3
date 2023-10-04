#pragma once

#include <QThread>
#include <Python.h>
#include <QtNetwork/qtcpsocket.h>

class IdentityThread  : public QThread
{
	Q_OBJECT

public:
	IdentityThread(QString,QString,QTcpSocket*);
	~IdentityThread();

private:
	char* QStringToChar(QString);

	QString Filename;
	QString DirPath;
	QTcpSocket* Tmpsocket = nullptr;

protected:
	void run();

signals:
	void SendEnd(QString,QTcpSocket*);
	void SendError(QString);
};

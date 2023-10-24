#pragma once

#include <QThread>
#include "Python/include/Python.h"

class IdentityThread  : public QThread
{
	Q_OBJECT

public:
    IdentityThread(QString,QString,qintptr);
	~IdentityThread();

private:
	char* QStringToChar(QString);

	QString Filename;
	QString DirPath;
    qintptr socketDesc;

protected:
	void run();

signals:
    void SendEnd(QString,qintptr);
	void SendError(QString);
};

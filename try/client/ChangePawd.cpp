#include "ChangePawd.h"

ChangePawd::ChangePawd(QWidget *parent,QString username,QString password)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConnect();
	Oldpassword = password;
	Username = username;
}

ChangePawd::~ChangePawd()
{}

/**
 * @brief 处理槽连接
 */
void ChangePawd::MyConnect() {
	connect(ui.QuitButton, &QPushButton::clicked, this, [this](){IsQiut = true; ChangePd(); });
	connect(ui.AcceptButton, &QPushButton::clicked, this, &ChangePawd::ChangePd);
}

/**
 * @brief 处理修改密码
 */
void ChangePawd::ChangePd() {
	if (IsQiut) {
		close();
		return;
	}
	if (Oldpassword == "") {
		QMessageBox::information(this, "错误", "获取原密码失败");
		return;
	}
	if (ui.OPlineEdit->text().compare(Oldpassword) != 0) {
		QMessageBox::information(this, "错误", "原密码错误");
		return;
	}
	emit(SendNP(ui.NPlineEdit->text()));
	//qDebug() << "发送新密码" << ui.NPlineEdit->text();
	close();
	return;
}

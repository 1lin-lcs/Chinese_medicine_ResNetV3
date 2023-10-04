#include "UserInfo.h"

UserInfo::UserInfo(QWidget*parent,QString username,QString passsword,QString email)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConnect();
	Username = username;
	Email = email;
	ui.UserInfo->setText(Username);
	ui.EmailInfo->setText(Email);
	Password = passsword;
}

UserInfo::~UserInfo()
{}


/**
 * @brief 处理槽函数
 */
void UserInfo::MyConnect() {
	connect(ui.ChangePassword, &QPushButton::clicked, this, &UserInfo::CPassword);
	connect(ui.LogoutButton, &QPushButton::clicked, this, [this] {emit(UserLogout()); close(); });
}

/**
 * @brief 处理修改密码
 */
void UserInfo::CPassword() {
	ChangePawd* dialog = new ChangePawd(nullptr,Username,Password);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
	connect(dialog, &ChangePawd::SendNP, this, &UserInfo::SendPassword);
}

/**
 * @brief 发送新密码
 */
void UserInfo::SendPassword(QString newpassword) {
	emit(SendPa(newpassword));
}






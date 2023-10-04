#include "Sign.h"

Sign::Sign(QWidget *parent,int index=0)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConnect();
	ui.SignTabWidget->setCurrentIndex(index);
}

Sign::~Sign()
{}

/**
 * @brief 注册槽函数
 */
void Sign::MyConnect() {
	connect(ui.AcceptButton, &QPushButton::clicked, this, &Sign::Accept);
}

/**
 * @brief 处理确定
 */
void Sign::Accept() {
	if (ui.SignTabWidget->currentIndex() == 0) {
		info << "0" << ui.SignUpUsername->text() << ui.SignUpPasswprd->text();
		emit(SendInfo(info));
	}
	else if(ui.SignTabWidget->currentIndex() == 1) {
		info << "1" << ui.SignInUsername->text() << ui.SignInPassword->text() << ui.SignInEmail->text();
		emit(SendInfo(info));
	}
	info.clear();
	close();
}
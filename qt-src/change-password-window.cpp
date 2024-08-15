/**
 * @file qt-src/change-password-window.cpp
 * @brief Implementation of class ChangePasswordWindow
 * 
 * @details
 * This file contains the implementation of the ChangePasswordWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/change-password-window.hpp"
#include "carinfo-manager/login-window.hpp"
#include <QMessageBox>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/httphandler-client.hpp"

ChangePasswordWindow::ChangePasswordWindow(QWidget *parent,
										   const std::string &ip,
										   const int port,
										   const Account &acc,
										   QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("修改密码");
	this->setFixedSize(320, 320);

	titleLabel = new QLabel("修改密码", this);
	titleLabel->setGeometry(110, 50, 100, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(250, 20, 50, 15);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(250, 40, 50, 15);
    accounttypeLabel->setAlignment(Qt::AlignCenter);

	oldPasswordLabel = new QLabel("原密码：", this);
	oldPasswordLabel->setGeometry(50, 100, 60, 30);
	oldPasswordEdit = new QLineEdit(this);
	oldPasswordEdit->setEchoMode(QLineEdit::Password);
	oldPasswordEdit->setGeometry(120, 100, 140, 30);

	newPasswordLabel = new QLabel("新密码：", this);
	newPasswordLabel->setGeometry(50, 140, 60, 30);
	newPasswordEdit = new QLineEdit(this);
	newPasswordEdit->setEchoMode(QLineEdit::Password);
	newPasswordEdit->setGeometry(120, 140, 140, 30);

	confirmPasswordLabel = new QLabel("确认密码：", this);
	confirmPasswordLabel->setGeometry(50, 180, 60, 30);
	confirmPasswordEdit = new QLineEdit(this);
	confirmPasswordEdit->setEchoMode(QLineEdit::Password);
	confirmPasswordEdit->setGeometry(120, 180, 140, 30);

	changePasswordButton = new QPushButton("修改", this);
	changePasswordButton->setGeometry(110, 230, 100, 30);
	connect(this->changePasswordButton,
			&QPushButton::clicked,
			this,
			&ChangePasswordWindow::changePassword);
}

ChangePasswordWindow::~ChangePasswordWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void ChangePasswordWindow::changePassword() {
	std::string oldPassword = oldPasswordEdit->text().toStdString();
	std::string newPassword = newPasswordEdit->text().toStdString();
	std::string confirmPassword = confirmPasswordEdit->text().toStdString();

	if (newPassword != confirmPassword) {
		QMessageBox::warning(this, "错误", "两次输入的密码不一致");
		return;
	}

	Hash oldPasswordHash(oldPassword, "Carinfo-Manager");
	Hash newPasswordHash(newPassword, "Carinfo-Manager");
	Account testAcc(acc.getUsername(), oldPasswordHash.getHash(), acc.getAccountType());

	auto res = ClientHttpHandler::handler_change_password(ip, port, testAcc, newPasswordHash.getHash());
	if (res) {
		QMessageBox::information(this, "成功", "修改密码成功，请重新登录");
		removeFromParent();
		LoginWindow *loginWindow = new LoginWindow(nullptr, ip, port);
		loginWindow->show();
		parentWindow->close();
		this->close();
	}
	else
		QMessageBox::critical(
			this, "错误", std::string("修改密码失败\n错误信息：" + res.message).c_str());
}

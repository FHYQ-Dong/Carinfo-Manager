/**
 * @file qt-src/update-account-window.cpp
 * @brief Implementation of class UpdateAccountWindow
 * 
 * @details
 * This file contains the implementation of the UpdateAccountWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/update-account-window.hpp"
#include <QMessageBox>
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/log.hpp"

UpdateAccountWindow::UpdateAccountWindow(QWidget *parent,
										 const std::string &ip,
										 const int port,
										 const Account &acc,
										 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("修改账户");
	this->setFixedSize(320, 370);

	titleLabel = new QLabel("修改账户", this);
	titleLabel->setGeometry(110, 50, 100, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(250, 20, 50, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(250, 40, 50, 30);
    accounttypeLabel->setAlignment(Qt::AlignCenter);

	targetUsernameLabel = new QLabel("原用户名：", this);
	targetUsernameLabel->setGeometry(50, 100, 60, 30);
	targetUsernameEdit = new QLineEdit(this);
	targetUsernameEdit->setGeometry(120, 100, 140, 30);

	newUsernameLabel = new QLabel("新用户名：", this);
	newUsernameLabel->setGeometry(50, 140, 60, 30);
	newUsernameEdit = new QLineEdit(this);
	newUsernameEdit->setGeometry(120, 140, 140, 30);

	newPasswordLabel = new QLabel("新密码：", this);
	newPasswordLabel->setGeometry(50, 180, 60, 30);
	newPasswordEdit = new QLineEdit(this);
	newPasswordEdit->setEchoMode(QLineEdit::Password);
	newPasswordEdit->setGeometry(120, 180, 140, 30);

	confirmPasswordLabel = new QLabel("确认密码：", this);
	confirmPasswordLabel->setGeometry(50, 220, 60, 30);
	confirmPasswordEdit = new QLineEdit(this);
	confirmPasswordEdit->setEchoMode(QLineEdit::Password);
	confirmPasswordEdit->setGeometry(120, 220, 140, 30);

	newAccounttypeLabel = new QLabel("新账户类型：", this);
	newAccounttypeLabel->setGeometry(50, 260, 70, 30);
	newAccounttypeComboBox = new QComboBox(this);
	newAccounttypeComboBox->setGeometry(130, 260, 130, 30);
	newAccounttypeComboBox->addItem("用户");
	newAccounttypeComboBox->addItem("管理员");

	updateAccountButton = new QPushButton("修改", this);
	updateAccountButton->setGeometry(110, 310, 100, 30);
	connect(updateAccountButton, &QPushButton::clicked, this, &UpdateAccountWindow::updateAccount);
}

UpdateAccountWindow::~UpdateAccountWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void UpdateAccountWindow::updateAccount() {
	std::string targetUsername = this->targetUsernameEdit->text().toStdString();
	std::string newUsername = this->newUsernameEdit->text().toStdString();
	std::string newPassword = this->newPasswordEdit->text().toStdString();
	std::string confirmPassword = this->confirmPasswordEdit->text().toStdString();
	Account::AccountType newAccountType = this->newAccounttypeComboBox->currentIndex() == 0
											  ? Account::AccountType::USER
											  : Account::AccountType::ADMIN;
	if (newPassword != confirmPassword) {
		QMessageBox::critical(this, "错误", "两次输入的密码不一致");
		return;
	}
	if (newUsername.empty() || newPassword.empty()) {
		QMessageBox::critical(this, "错误", "用户名和密码不能为空");
		return;
	}

	Hash passwordHash(newPassword, "Carinfo-Manager");
	std::string newPasswdHash = passwordHash.getHash();

	auto res = ClientHttpHandler::handler_update_account(this->ip,
														 this->port,
														 this->acc,
														 targetUsername,
														 newUsername,
														 newPasswdHash,
														 newAccountType);
	if (res)
		QMessageBox::information(this, "成功", "修改账户成功");
	else
		QMessageBox::critical(
			this, "错误", std::string("修改账户失败\n错误信息：" + res.message).c_str());
}

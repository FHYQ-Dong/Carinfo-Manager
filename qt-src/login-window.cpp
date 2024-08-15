/**
 * @file qt-src/login-window.cpp
 * @brief Implementation of class LoginWindow
 * 
 * @details
 * This file contains the implementation of the LoginWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/login-window.hpp"
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/manage-window.hpp"
#include "carinfo-manager/search-car-window.hpp"
#include "carinfo-manager/select-server-window.hpp"

LoginWindow::LoginWindow(QWidget *parent, const std::string &ip, const int port)
	: QWidget(parent), ip(ip), port(port) {
	this->setWindowTitle("登录");
	this->setFixedSize(400, 300);

	titleLabel = new QLabel("车辆管理系统", this);
	titleLabel->setGeometry(100, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel("用户名：", this);
	usernameLabel->setGeometry(50, 100, 100, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	usernameEdit = new QLineEdit(this);
	usernameEdit->setGeometry(150, 100, 200, 30);
	usernameEdit->setPlaceholderText("请输入用户名");

	passwordLabel = new QLabel("密码：", this);
	passwordLabel->setGeometry(50, 150, 100, 30);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordEdit = new QLineEdit(this);
	passwordEdit->setGeometry(150, 150, 200, 30);
	passwordEdit->setEchoMode(QLineEdit::Password);
	passwordEdit->setPlaceholderText("请输入密码");

	loginButton = new QPushButton("登录", this);
	loginButton->setGeometry(200, 200, 100, 30);
	connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);
	returnButton = new QPushButton("返回", this);
	returnButton->setGeometry(100, 200, 100, 30);
	connect(returnButton, &QPushButton::clicked, this, &LoginWindow::returnToSelectServer);
}

LoginWindow::~LoginWindow() {}

void LoginWindow::login() {
	if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
		QMessageBox::critical(this, "错误", "用户名或密码不能为空");
		return;
	}
	try {
		username = usernameEdit->text().toStdString();
		password = passwordEdit->text().toStdString();
	}
	catch (std::exception &e) {
		QMessageBox::critical(this, "错误", "不合法的用户名或密码");
		return;
	}
	Hash passwordHash(password, "Carinfo-Manager");
	Account acc(username, passwordHash.getHash(), Account::AccountType::NONETYPE);
	ClientHttpHandler::AccountVerifyResult result = ClientHttpHandler::handler_login(ip, port, acc);
	if (result == ClientHttpHandler::AccountVerifyResult::SUCCESS) {
		QMessageBox::information(this, "成功", "登录成功");
		if (acc.getAccountType() == Account::AccountType::ADMIN ||
			acc.getAccountType() == Account::AccountType::USER) {
			ManageWindow *manageWindow = new ManageWindow(nullptr, ip, port, acc);
			this->close();
			manageWindow->show();
			return;
		}
		else
			QMessageBox::critical(this, "错误", "未知账户类型");
	}
	else if (result ==
			 ClientHttpHandler::AccountVerifyResult::ACCOUNT_NOT_FOUND_OR_WRONG_PASSWORD) {
		QMessageBox::critical(this, "错误", "用户名或密码错误");
	}
	else if (result == ClientHttpHandler::AccountVerifyResult::NETWORK_ERROR) {
		QMessageBox::critical(this, "错误", "无法连接到服务器");
	}
	else if (result == ClientHttpHandler::AccountVerifyResult::BAD_REQUEST) {
		QMessageBox::critical(this, "错误", "请求错误");
	}
	else {
		QMessageBox::critical(this, "错误", "未知错误");
	}
}

void LoginWindow::returnToSelectServer() {
	SelectServerWindow *selectServerWindow = new SelectServerWindow;
	this->close();
	selectServerWindow->show();
}

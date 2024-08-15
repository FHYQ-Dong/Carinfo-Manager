/**
 * @file qt-src/select-server-window.cpp
 * @brief Implementation of class SelectServerWindow
 * 
 * @details
 * This file contains the implementation of the SelectServerWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/select-server-window.hpp"
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/login-window.hpp"
#include "carinfo-manager/search-car-window.hpp"

SelectServerWindow::SelectServerWindow(QWidget *parent) : QWidget(parent) {
	this->setWindowTitle("选择服务器");
	this->setFixedSize(400, 300);

	titleLabel = new QLabel("车辆管理系统", this);
	titleLabel->setGeometry(100, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	serverLabel = new QLabel("服务器地址：", this);
	serverLabel->setGeometry(50, 100, 100, 30);
	serverLabel->setAlignment(Qt::AlignCenter);
	serverEdit = new QLineEdit(this);
	serverEdit->setGeometry(150, 100, 200, 30);
	serverEdit->setPlaceholderText("请输入服务器地址");

	portLabel = new QLabel("端口：", this);
	portLabel->setGeometry(50, 150, 100, 30);
	portLabel->setAlignment(Qt::AlignCenter);
	portEdit = new QLineEdit(this);
	portEdit->setGeometry(150, 150, 200, 30);
	portEdit->setPlaceholderText("请输入端口");

	verifyButton = new QPushButton("验证", this);
	verifyButton->setGeometry(150, 200, 100, 30);
	connect(verifyButton, &QPushButton::clicked, this, &SelectServerWindow::verifyServer);
}

SelectServerWindow::~SelectServerWindow() {}

void SelectServerWindow::verifyServer() {
	if (serverEdit->text().isEmpty() || portEdit->text().isEmpty()) {
		QMessageBox::critical(this, "错误", "服务器地址或端口不能为空");
		return;
	}
	try {
		ip = serverEdit->text().toStdString();
		port = portEdit->text().toInt();
	}
	catch (std::exception &e) {
		QMessageBox::critical(this, "错误", "不合法的地址或端口");
		return;
	}

	bool isServerValid = ClientHttpHandler::handler_verify_server(ip, port);
	if (isServerValid) {
		LoginWindow *loginWindow = new LoginWindow(nullptr, ip, port);
		this->close();
		loginWindow->show();
	}
	else {
		QMessageBox::critical(this, "错误", "无法连接到服务器");
	}
}

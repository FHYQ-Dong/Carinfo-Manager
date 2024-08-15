/**
 * @file qt-src/remove-account-window.cpp
 * @brief Implementation of class RemoveAccountWindow
 * 
 * @details
 * This file contains the implementation of the RemoveAccountWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/remove-account-window.hpp"
#include <QMessageBox>
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/log.hpp"

RemoveAccountWindow::RemoveAccountWindow(QWidget *parent,
										 const std::string &ip,
										 const int port,
										 const Account &acc,
										 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("删除账户");
    this->setFixedSize(300, 300);

    titleLabel = new QLabel("删除账户", this);
    titleLabel->setGeometry(50, 50, 200, 30);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

    usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
    usernameLabel->setGeometry(240, 20, 50, 30);
    usernameLabel->setAlignment(Qt::AlignCenter);
    accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
    accounttypeLabel->setGeometry(240, 40, 50, 30);
    accounttypeLabel->setAlignment(Qt::AlignCenter);

    targetUsernameLabel = new QLabel("目标用户名：", this);
    targetUsernameLabel->setGeometry(50, 100, 70, 30);
    targetUsernameEdit = new QLineEdit(this);
    targetUsernameEdit->setGeometry(130, 100, 120, 30);

    removeButton = new QPushButton("删除", this);
    removeButton->setGeometry(100, 150, 100, 30);
    connect(removeButton, &QPushButton::clicked, this, &RemoveAccountWindow::removeAccount);
}

RemoveAccountWindow::~RemoveAccountWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void RemoveAccountWindow::removeAccount() {
	std::string target_username = this->acc.getUsername();
	if (target_username.empty()) {
		QMessageBox::warning(this, "错误", "用户名不能为空");
		return;
	}
    if (target_username == this->acc.getUsername()) {
        QMessageBox::warning(this, "错误", "不能删除自己");
        return;
    }
	auto res = ClientHttpHandler::handler_remove_account(
		this->ip, this->port, this->acc, target_username);
	if (res)
		QMessageBox::information(this, "成功", "删除账户成功");
	else
		QMessageBox::critical(
			this, "错误", std::string("删除账户失败\n错误信息：" + res.message).c_str());
}

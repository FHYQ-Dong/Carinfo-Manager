/**
 * @file qt-src/manage-account-window.cpp
 * @brief Implementation of class ManageAccountWindow
 * 
 * @details
 * This file contains the implementation of the ManageAccountWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/manage-account-window.hpp"
#include "carinfo-manager/add-account-window.hpp"
#include "carinfo-manager/log.hpp"
#include "carinfo-manager/remove-account-window.hpp"
#include "carinfo-manager/update-account-window.hpp"

ManageAccountWindow::ManageAccountWindow(QWidget *parent,
										 const std::string &ip,
										 const int port,
										 const Account &acc,
										 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("账户管理");
	this->setFixedSize(300, 330);

	titleLabel = new QLabel("账户管理", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 15);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 15);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	addAccountButton = new QPushButton("添加账户", this);
	addAccountButton->setGeometry(50, 100, 200, 30);
	connect(addAccountButton, &QPushButton::clicked, this, &ManageAccountWindow::addAccount);

	updateAccountButton = new QPushButton("修改账户", this);
	updateAccountButton->setGeometry(50, 140, 200, 30);
	connect(updateAccountButton, &QPushButton::clicked, this, &ManageAccountWindow::updateAccount);
    
	removeAccountButton = new QPushButton("删除账户", this);
	removeAccountButton->setGeometry(50, 180, 200, 30);
	connect(removeAccountButton, &QPushButton::clicked, this, &ManageAccountWindow::removeAccount);


}

ManageAccountWindow::~ManageAccountWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void ManageAccountWindow::addAccount() {
	AddAccountWindow *addAccountWindow = new AddAccountWindow(nullptr, ip, port, acc, this);
	addSubWindow(addAccountWindow);
	addAccountWindow->show();
}

void ManageAccountWindow::removeAccount() {
	RemoveAccountWindow *removeAccountWindow =
		new RemoveAccountWindow(nullptr, ip, port, acc, this);
	addSubWindow(removeAccountWindow);
	removeAccountWindow->show();
}

void ManageAccountWindow::updateAccount() {
	UpdateAccountWindow *updateAccountWindow =
		new UpdateAccountWindow(nullptr, ip, port, acc, this);
	addSubWindow(updateAccountWindow);
	updateAccountWindow->show();
}

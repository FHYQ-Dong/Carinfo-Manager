/**
 * @file qt-src/manage-window.cpp
 * @brief Implementation of class ManageWindow
 * 
 * @details
 * This file contains the implementation of the ManageWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/manage-window.hpp"
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/login-window.hpp"
#include "carinfo-manager/manage-account-window.hpp"
#include "carinfo-manager/manage-car-window.hpp"
#include "carinfo-manager/search-account-window.hpp"
#include "carinfo-manager/search-car-window.hpp"
#include "carinfo-manager/change-password-window.hpp"

ManageWindow::ManageWindow(QWidget *parent,
						   const std::string &ip,
						   const int port,
						   const Account &acc)
	: QWidgetWithSubWindows(parent), ip(ip), port(port), acc(acc) {
	this->setWindowTitle(acc.isAdmin() ? "管理员管理" : "用户查询");
	this->setFixedSize(300, 330);

	titleLabel = new QLabel(acc.isAdmin() ? "管理员管理" : "用户查询", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 15);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 15);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	if (acc.isAdmin()) {
        carManageButton = new QPushButton("车辆管理", this);
		carManageButton->setGeometry(50, 100, 200, 30);
		connect(carManageButton, &QPushButton::clicked, this, &ManageWindow::carManage);
		
        accountManageButton = new QPushButton("账户管理", this);
		accountManageButton->setGeometry(50, 140, 200, 30);
		connect(accountManageButton, &QPushButton::clicked, this, &ManageWindow::accountManage);

		carSearchButton = new QPushButton("查询车辆", this);
		carSearchButton->setGeometry(50, 180, 200, 30);
		connect(carSearchButton, &QPushButton::clicked, this, &ManageWindow::carSearch);

		accountSearchButton = new QPushButton("查询账户", this);
		accountSearchButton->setGeometry(50, 220, 200, 30);
		connect(accountSearchButton, &QPushButton::clicked, this, &ManageWindow::accountSearch);

        changePasswordButton = new QPushButton("修改密码", this);
        changePasswordButton->setGeometry(50, 260, 95, 30);
        connect(changePasswordButton, &QPushButton::clicked, this, &ManageWindow::changePassword);

		logoutButton = new QPushButton("注销", this);
		logoutButton->setGeometry(155, 260, 95, 30);
		connect(logoutButton, &QPushButton::clicked, this, &ManageWindow::logout);
	}
	else {
		carSearchButton = new QPushButton("查询车辆", this);
		carSearchButton->setGeometry(50, 140, 200, 30);
		connect(carSearchButton, &QPushButton::clicked, this, &ManageWindow::carSearch);

        changePasswordButton = new QPushButton("修改密码", this);
        changePasswordButton->setGeometry(50, 260, 95, 30);
        connect(changePasswordButton, &QPushButton::clicked, this, &ManageWindow::changePassword);

		logoutButton = new QPushButton("注销", this);
		logoutButton->setGeometry(155, 260, 95, 30);
		connect(logoutButton, &QPushButton::clicked, this, &ManageWindow::logout);
	}
}

ManageWindow::~ManageWindow() {
	closeAllSubWindows();
}

void ManageWindow::accountManage() {
	ManageAccountWindow *manageAccountWindow =
		new ManageAccountWindow(nullptr, ip, port, acc, this);
	addSubWindow(manageAccountWindow);
	manageAccountWindow->show();
}

void ManageWindow::carManage() {
	ManageCarWindow *manageCarWindow = new ManageCarWindow(nullptr, ip, port, acc, this);
	addSubWindow(manageCarWindow);
	manageCarWindow->show();
}

void ManageWindow::accountSearch() {
	SearchAccountWindow *searchAccountWindow =
		new SearchAccountWindow(nullptr, ip, port, acc, this);
	addSubWindow(searchAccountWindow);
	searchAccountWindow->show();
}

void ManageWindow::carSearch() {
	SearchCarWindow *searchCarWindow = new SearchCarWindow(nullptr, ip, port, acc, this);
	addSubWindow(searchCarWindow);
	searchCarWindow->show();
}

void ManageWindow::changePassword() {
    ChangePasswordWindow *changePasswordWindow =
        new ChangePasswordWindow(nullptr, ip, port, acc, this);
    addSubWindow(changePasswordWindow);
    changePasswordWindow->show();
}

void ManageWindow::logout() {
	LoginWindow *loginWindow = new LoginWindow(nullptr, ip, port);
	loginWindow->show();
	closeAllSubWindows();
	this->close();
}

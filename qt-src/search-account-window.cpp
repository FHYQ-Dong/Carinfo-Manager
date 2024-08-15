/**
 * @file qt-src/search-account-window.cpp
 * @brief Implementation of class SearchAccountWindow
 * 
 * @details
 * This file contains the implementation of the SearchAccountWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/search-account-window.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include "carinfo-manager/display-accountinfo-window.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "json/json.hpp"

using json = nlohmann::json;

SearchAccountWindow::SearchAccountWindow(QWidget *parent,
										 const std::string &ip,
										 const int port,
										 const Account &acc,
										 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("查询账户");
	this->setFixedSize(300, 300);

	titleLabel = new QLabel("查询账户", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 30);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	searchUsernameLabel = new QLabel("用户名：", this);
	searchUsernameLabel->setGeometry(50, 100, 70, 30);
	searchUsernameLineEdit = new QLineEdit(this);
	searchUsernameLineEdit->setGeometry(120, 100, 150, 30);

	searchButton = new QPushButton("查询", this);
	searchButton->setGeometry(100, 150, 100, 30);
	connect(searchButton, &QPushButton::clicked, this, &SearchAccountWindow::searchAccount);

	saveAllAccountsButton = new QPushButton("保存所有账户", this);
	saveAllAccountsButton->setGeometry(100, 190, 100, 30);
	connect(
		saveAllAccountsButton, &QPushButton::clicked, this, &SearchAccountWindow::saveAllAccounts);
}

SearchAccountWindow::~SearchAccountWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void SearchAccountWindow::searchAccount() {
	std::string searchUsername = searchUsernameLineEdit->text().toStdString();
	if (searchUsername.empty()) {
		QMessageBox::critical(this, "错误", "用户名不能为空");
		return;
	}

	auto res = ClientHttpHandler::handler_search_account(ip, port, acc, searchUsername);
	if (!res) {
		QMessageBox::critical(
			this, "错误", std::string("查询账户失败\n错误信息：" + res.message).c_str());
		return;
	}
	std::stringstream ss;
	ss << res.json_obj.dump(4);
	AccountPool accountpool;
	accountpool.load(ss);

	DisplayAccountinfoWindow *displayAccountinfoWindow =
		new DisplayAccountinfoWindow(nullptr, accountpool, searchUsername, ip, port, acc, this);
	addSubWindow(displayAccountinfoWindow);
	displayAccountinfoWindow->show();
}

void SearchAccountWindow::saveAllAccounts() {
	auto res = ClientHttpHandler::handler_get_all_accounts(ip, port, acc);
	if (!res) {
		QMessageBox::critical(
			this, "错误", std::string("获取所有账户失败\n错误信息：" + res.message).c_str());
		return;
	}

	std::string filename =
		QFileDialog::getSaveFileName(this, "保存文件", "", "JOSN文件(*.json)").toLocal8Bit().data();
	if (filename.empty()) {
		QMessageBox::critical(this, "错误", "文件名不合法");
		return;
	}

	std::ofstream file(filename, std::ios::out);
	if (!file.is_open()) {
		QMessageBox::critical(this, "错误", "无法打开文件");
		return;
	}
	file << res.json_obj.dump(4);
	file.close();
	QMessageBox::information(this, "成功", "保存成功");
}

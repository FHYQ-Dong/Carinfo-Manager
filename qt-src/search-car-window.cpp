/**
 * @file qt-src/search-car-window.cpp
 * @brief Implementation of class SearchCarWindow
 * 
 * @details
 * This file contains the implementation of the SearchCarWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/search-car-window.hpp"
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/display-carinfo-window.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "json/json.hpp"

using json = nlohmann::json;

SearchCarWindow::SearchCarWindow(QWidget *parent,
								 const std::string &ip,
								 const int port,
								 const Account &acc,
								 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("查找车辆");
	this->setFixedSize(300, 300);

	titleLabel = new QLabel("查询车辆信息", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 15);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 15);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	caridLabel = new QLabel("车牌号：", this);
	caridLabel->setGeometry(50, 100, 50, 30);
	caridLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	carownerLabel = new QLabel("车主：", this);
	carownerLabel->setGeometry(50, 130, 50, 30);
	carownerLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	cartypeLabel = new QLabel("车型：", this);
	cartypeLabel->setGeometry(50, 160, 50, 30);
	cartypeLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	carcolorLabel = new QLabel("颜色：", this);
	carcolorLabel->setGeometry(50, 190, 50, 30);
	carcolorLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	caridEdit = new QLineEdit(this);
	caridEdit->setGeometry(110, 100, 100, 30);
	carownerEdit = new QLineEdit(this);
	carownerEdit->setGeometry(110, 130, 100, 30);
	cartypeEdit = new QLineEdit(this);
	cartypeEdit->setGeometry(110, 160, 100, 30);
	carcolorEdit = new QLineEdit(this);
	carcolorEdit->setGeometry(110, 190, 100, 30);

	searchButton = new QPushButton("查询", this);
	searchButton->setGeometry(100, 240, 100, 30);
	connect(searchButton, &QPushButton::clicked, this, &SearchCarWindow::search);
}

SearchCarWindow::~SearchCarWindow() {
	removeFromParent();
}

void SearchCarWindow::search() {
	std::string car_id = caridEdit->text().toStdString();
	std::string car_owner = carownerEdit->text().toStdString();
	std::string car_type = cartypeEdit->text().toStdString();
	std::string car_color = carcolorEdit->text().toStdString();

	auto res = ClientHttpHandler::handler_get_carinfo(
		ip, port, acc, car_id, car_owner, car_color, car_type);
	if (!res) {
		QMessageBox::critical(
			this, "错误", std::string("查询失败\n错误信息：" + res.message).c_str());
		return;
	}

	std::stringstream ss;
	ss << res.json_obj.dump(4);
	CarPool searchRes;
	searchRes.load(ss);

	DisplayCarInfoWindow *displayCarinfoWindow =
		new DisplayCarInfoWindow(nullptr,
								 searchRes,
								 SearchCarQuery(car_id, car_owner, car_type, car_color),
								 ip,
								 port,
								 acc,
								 this);
	addSubWindow(displayCarinfoWindow);
	displayCarinfoWindow->show();
}

/**
 * @file qt-src/remove-car-window.cpp
 * @brief Implementation of class RemoveCarWindow
 * 
 * @details
 * This file contains the implementation of the RemoveCarWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/remove-car-window.hpp"
#include <QMessageBox>

RemoveCarWindow::RemoveCarWindow(QWidget *parent,
								 const std::string &ip,
								 const int port,
								 const Account &acc,
								 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("删除车辆");
	this->setFixedSize(300, 300);

	titleLabel = new QLabel("删除车辆", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 30);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	carIdLabel = new QLabel("车牌号：", this);
	carIdLabel->setGeometry(50, 100, 70, 30);
	carIdLineEdit = new QLineEdit(this);
	carIdLineEdit->setGeometry(120, 100, 150, 30);

	removeCarButton = new QPushButton("删除", this);
	removeCarButton->setGeometry(100, 150, 100, 30);
	connect(removeCarButton, &QPushButton::clicked, this, &RemoveCarWindow::removeCar);
}

RemoveCarWindow::~RemoveCarWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void RemoveCarWindow::removeCar() {
	std::string car_id = carIdLineEdit->text().toStdString();
	if (car_id.empty()) {
		QMessageBox::warning(this, "错误", "车牌号不能为空");
		return;
	}

	auto res = ClientHttpHandler::handler_remove_car(ip, port, acc, car_id);
	if (res) {
		QMessageBox::information(this, "成功", "车辆删除成功");
	}
	else {
		QMessageBox::critical(
			this, "错误", std::string("车辆删除失败\n错误信息：" + res.message).c_str());
	}
}

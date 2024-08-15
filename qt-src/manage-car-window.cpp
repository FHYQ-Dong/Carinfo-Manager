/**
 * @file qt-src/manage-car-window.cpp
 * @brief Implementation of class ManageCarWindow
 * 
 * @details
 * This file contains the implementation of the ManageCarWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/manage-car-window.hpp"
#include "carinfo-manager/add-car-window.hpp"
#include "carinfo-manager/remove-car-window.hpp"
#include "carinfo-manager/update-car-window.hpp"

ManageCarWindow::ManageCarWindow(QWidget *parent,
								 const std::string &ip,
								 const int port,
								 const Account &acc,
								 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("车辆管理");
	this->setFixedSize(300, 330);

	titleLabel = new QLabel("车辆管理", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(240, 20, 50, 15);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(240, 40, 50, 15);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	addCarButton = new QPushButton("添加车辆", this);
	addCarButton->setGeometry(50, 100, 200, 30);
	connect(addCarButton, &QPushButton::clicked, this, &ManageCarWindow::addCar);

	updateCarButton = new QPushButton("修改车辆", this);
	updateCarButton->setGeometry(50, 140, 200, 30);
	connect(updateCarButton, &QPushButton::clicked, this, &ManageCarWindow::updateCar);
    
    removeCarButton = new QPushButton("删除车辆", this);
	removeCarButton->setGeometry(50, 180, 200, 30);
	connect(removeCarButton, &QPushButton::clicked, this, &ManageCarWindow::removeCar);
}

ManageCarWindow::~ManageCarWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void ManageCarWindow::addCar() {
	AddCarWindow *addCarWindow = new AddCarWindow(nullptr, ip, port, acc, this);
	addSubWindow(addCarWindow);
	addCarWindow->show();
}

void ManageCarWindow::removeCar() {
	RemoveCarWindow *removeCarWindow = new RemoveCarWindow(nullptr, ip, port, acc, this);
	addSubWindow(removeCarWindow);
	removeCarWindow->show();
}

void ManageCarWindow::updateCar() {
	UpdateCarWindow *updateCarWindow = new UpdateCarWindow(nullptr, ip, port, acc, this);
	addSubWindow(updateCarWindow);
	updateCarWindow->show();
}

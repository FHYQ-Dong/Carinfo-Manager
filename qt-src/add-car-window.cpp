/**
 * @file qt-src/add-car-window.cpp
 * @brief Implementation of class AddCarWindow
 * 
 * @details
 * This file contains the implementation of the AddCarWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/add-car-window.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <string>
#include "carinfo-manager/httphandler-client.hpp"

AddCarWindow::AddCarWindow(QWidget *parent,
						   const std::string &ip,
						   const int port,
						   const Account &acc,
						   QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("添加车辆");
	this->setFixedSize(320, 400);

	titleLabel = new QLabel("添加车辆", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(250, 20, 50, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(250, 40, 50, 30);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	carIdLabel = new QLabel("车牌号：", this);
	carIdLabel->setGeometry(50, 100, 70, 30);
	carIdLineEdit = new QLineEdit(this);
	carIdLineEdit->setGeometry(120, 100, 150, 30);

	carOwnerLabel = new QLabel("车主：", this);
	carOwnerLabel->setGeometry(50, 140, 70, 30);
	carOwnerLineEdit = new QLineEdit(this);
	carOwnerLineEdit->setGeometry(120, 140, 150, 30);

	carTypeLabel = new QLabel("车型：", this);
	carTypeLabel->setGeometry(50, 180, 70, 30);
	carTypeLineEdit = new QLineEdit(this);
	carTypeLineEdit->setGeometry(120, 180, 150, 30);

	carColorLabel = new QLabel("颜色：", this);
	carColorLabel->setGeometry(50, 220, 70, 30);
	carColorLineEdit = new QLineEdit(this);
	carColorLineEdit->setGeometry(120, 220, 150, 30);

	carYearLabel = new QLabel("年份：", this);
	carYearLabel->setGeometry(50, 260, 70, 30);
	carYearLineEdit = new QLineEdit(this);
	carYearLineEdit->setGeometry(120, 260, 150, 30);

	carImgPathLabel = new QLabel("图片路径：", this);
	carImgPathLabel->setGeometry(50, 300, 70, 30);
	carImgPathLineEdit = new QLineEdit(this);
	carImgPathLineEdit->setGeometry(120, 300, 110, 30);
	carImgSelectButton = new QPushButton("选择", this);
	carImgSelectButton->setGeometry(230, 300, 40, 30);
	connect(carImgSelectButton, &QPushButton::clicked, this, &AddCarWindow::getCarImagePath);

	addCarButton = new QPushButton("添加", this);
	addCarButton->setGeometry(110, 340, 100, 30);
	connect(addCarButton, &QPushButton::clicked, this, &AddCarWindow::addCar);
}

AddCarWindow::~AddCarWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void AddCarWindow::getCarImagePath() {
	QString imgPath =
		QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg)");
	if (!imgPath.isEmpty())
		carImgPathLineEdit->setText(imgPath);
}

void AddCarWindow::addCar() {
	if (carIdLineEdit->text().isEmpty() || carOwnerLineEdit->text().isEmpty() ||
		carTypeLineEdit->text().isEmpty() || carColorLineEdit->text().isEmpty() ||
		carYearLineEdit->text().isEmpty() || carImgPathLineEdit->text().isEmpty()) {
		QMessageBox::critical(this, "错误", "请填写完整信息");
		return;
	}
	std::string carId = carIdLineEdit->text().toStdString(),
				carOwner = carOwnerLineEdit->text().toStdString(),
				carType = carTypeLineEdit->text().toStdString(),
				carColor = carColorLineEdit->text().toStdString(),
				carImgPath = carImgPathLineEdit->text().toLocal8Bit().data(), carImgType = "",
				carImgData = "";
	int carYear = carYearLineEdit->text().toInt();

	if (carImgPath.find_last_of('.') != std::string::npos) {
		carImgType = carImgPath.substr(carImgPath.find_last_of('.'));
		if (carImgType != ".png" && carImgType != ".jpg" && carImgType != ".jpeg") {
			QMessageBox::critical(this, "错误", "图片格式不支持");
			return;
		}
	}
	else {
		QMessageBox::critical(this, "错误", "图片路径不合法");
		return;
	}

	std::ifstream imgFile(carImgPath, std::ios::binary);
	if (imgFile.is_open()) {
		carImgData =
			std::string(std::istreambuf_iterator<char>(imgFile), std::istreambuf_iterator<char>());
		imgFile.close();
	}
	else {
		QMessageBox::critical(this, "错误", "打开图片失败");
		return;
	}

	auto res = ClientHttpHandler::handler_add_car(
		ip, port, acc, carId, carOwner, carType, carColor, carYear, carImgData, carImgType);
	if (res)
		QMessageBox::information(this, "成功", "添加车辆成功");
	else
		QMessageBox::critical(
			this, "错误", std::string("添加车辆失败\n错误信息：" + res.message).c_str());
}

/**
 * @file qt-src/update-car-window.cpp
 * @brief Implementation of class UpdateCarWindow
 * 
 * @details
 * This file contains the implementation of the UpdateCarWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/update-car-window.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <string>
#include "carinfo-manager/httphandler-client.hpp"

UpdateCarWindow::UpdateCarWindow(QWidget *parent,
								 const std::string &ip,
								 const int port,
								 const Account &acc,
								 QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow), ip(ip), port(port), acc(acc) {
	this->setWindowTitle("修改车辆");
	this->setFixedSize(320, 460);

	titleLabel = new QLabel("修改车辆", this);
	titleLabel->setGeometry(50, 50, 200, 30);
	titleLabel->setAlignment(Qt::AlignCenter);
	titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

	usernameLabel = new QLabel(QString::fromStdString(acc.getUsername()), this);
	usernameLabel->setGeometry(250, 20, 50, 30);
	usernameLabel->setAlignment(Qt::AlignCenter);
	accounttypeLabel = new QLabel(acc.isAdmin() ? "管理员" : "用户", this);
	accounttypeLabel->setGeometry(250, 40, 50, 30);
	accounttypeLabel->setAlignment(Qt::AlignCenter);

	originalCarIdLabel = new QLabel("原车牌号：", this);
	originalCarIdLabel->setGeometry(50, 100, 70, 30);
	originalCarIdLineEdit = new QLineEdit(this);
	originalCarIdLineEdit->setGeometry(120, 100, 150, 30);

	newCarIdLabel = new QLabel("新车牌号：", this);
	newCarIdLabel->setGeometry(50, 140, 70, 30);
	newCarIdLineEdit = new QLineEdit(this);
	newCarIdLineEdit->setGeometry(120, 140, 150, 30);

	newCarOwnerLabel = new QLabel("新车主：", this);
	newCarOwnerLabel->setGeometry(50, 180, 70, 30);
	newCarOwnerLineEdit = new QLineEdit(this);
	newCarOwnerLineEdit->setGeometry(120, 180, 150, 30);

	newCarTypeLabel = new QLabel("新车型：", this);
	newCarTypeLabel->setGeometry(50, 220, 70, 30);
	newCarTypeLineEdit = new QLineEdit(this);
	newCarTypeLineEdit->setGeometry(120, 220, 150, 30);

	newCarColorLabel = new QLabel("新颜色：", this);
	newCarColorLabel->setGeometry(50, 260, 70, 30);
	newCarColorLineEdit = new QLineEdit(this);
	newCarColorLineEdit->setGeometry(120, 260, 150, 30);

	newCarYearLabel = new QLabel("新年份：", this);
	newCarYearLabel->setGeometry(50, 300, 70, 30);
	newCarYearLineEdit = new QLineEdit(this);
	newCarYearLineEdit->setGeometry(120, 300, 150, 30);

	newCarImgPathLabel = new QLabel("新图片路径：", this);
	newCarImgPathLabel->setGeometry(50, 340, 70, 30);
	newCarImgPathLineEdit = new QLineEdit(this);
	newCarImgPathLineEdit->setGeometry(120, 340, 110, 30);
	carImgSelectButton = new QPushButton("选择", this);
	carImgSelectButton->setGeometry(230, 340, 40, 30);
	connect(carImgSelectButton, &QPushButton::clicked, this, &UpdateCarWindow::getCarImgPath);

	updateCarButton = new QPushButton("修改", this);
	updateCarButton->setGeometry(110, 380, 100, 30);
	connect(updateCarButton, &QPushButton::clicked, this, &UpdateCarWindow::updateCar);
}

UpdateCarWindow::~UpdateCarWindow() {
	closeAllSubWindows();
	removeFromParent();
}

void UpdateCarWindow::getCarImgPath() {
	QString imgPath =
		QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg)");
	if (!imgPath.isEmpty())
		newCarImgPathLineEdit->setText(imgPath);
}

void UpdateCarWindow::updateCar() {
	if (originalCarIdLineEdit->text().isEmpty() || newCarIdLineEdit->text().isEmpty() ||
		newCarOwnerLineEdit->text().isEmpty() || newCarTypeLineEdit->text().isEmpty() ||
		newCarColorLineEdit->text().isEmpty() || newCarYearLineEdit->text().isEmpty() ||
		newCarImgPathLineEdit->text().isEmpty()) {
		QMessageBox::warning(this, "错误", "请填写完整信息");
		return;
	}

	std::string originalCarId = originalCarIdLineEdit->text().toStdString(),
				newCarId = newCarIdLineEdit->text().toStdString(),
				newCarOwner = newCarOwnerLineEdit->text().toStdString(),
				newCarType = newCarTypeLineEdit->text().toStdString(),
				newCarColor = newCarColorLineEdit->text().toStdString(),
				newCarImgPath = newCarImgPathLineEdit->text().toLocal8Bit().data(),
				newCarImgType = "", newCarImgData = "";
	int newCarYear = newCarYearLineEdit->text().toInt();

	if (newCarImgPath.find_last_of('.') != std::string::npos) {
		newCarImgType = newCarImgPath.substr(newCarImgPath.find_last_of('.'));
		if (newCarImgType != ".png" && newCarImgType != ".jpg" && newCarImgType != ".jpeg") {
			QMessageBox::critical(this, "错误", "图片格式不支持");
			return;
		}
	}
	else {
		QMessageBox::critical(this, "错误", "图片路径不合法");
		return;
	}

	std::ifstream imgFile(newCarImgPath, std::ios::binary);
	if (imgFile.is_open()) {
		newCarImgData =
			std::string(std::istreambuf_iterator<char>(imgFile), std::istreambuf_iterator<char>());
		imgFile.close();
	}
	else {
		QMessageBox::critical(this, "错误", "打开图片失败");
		return;
	}

	auto res = ClientHttpHandler::handler_update_car(ip,
													 port,
													 acc,
													 originalCarId,
													 newCarId,
													 newCarOwner,
													 newCarType,
													 newCarColor,
													 newCarYear,
													 newCarImgData,
													 newCarImgType);
	if (res)
		QMessageBox::information(this, "成功", "修改成功");
	else
		QMessageBox::critical(
			this, "错误", std::string("修改车辆失败\n错误信息：" + res.message).c_str());
}

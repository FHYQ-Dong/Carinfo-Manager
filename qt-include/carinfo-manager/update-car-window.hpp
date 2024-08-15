/**
 * @file qt-include/carinfo-manager/update-car-window.hpp
 * @brief Declaration of class UpdateCarWindow
 * 
 * @details
 * This file contains the declaration of the UpdateCarWindow class.
 * The UpdateCarWindow class provides a window to update a car.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class UpdateCarWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	UpdateCarWindow(QWidget *parent = nullptr,
					const std::string &ip = "",
					const int port = 0,
					const Account &acc = Account(),
					QWidgetWithSubWindows *parentWindow = nullptr);
	~UpdateCarWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
	QLabel *originalCarIdLabel, *newCarIdLabel, *newCarOwnerLabel, *newCarTypeLabel,
		*newCarColorLabel, *newCarYearLabel, *newCarImgPathLabel;
	QLineEdit *originalCarIdLineEdit, *newCarIdLineEdit, *newCarOwnerLineEdit, *newCarTypeLineEdit,
		*newCarColorLineEdit, *newCarYearLineEdit, *newCarImgPathLineEdit;
	QPushButton *carImgSelectButton, *updateCarButton;

  private slots:
	void getCarImgPath();
	void updateCar();
};

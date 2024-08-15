/**
 * @file qt-include/carinfo-manager/add-car-window.hpp
 * @brief Declaration of class AddCarWindow
 * 
 * @details
 * This file contains the declaration of the AddCarWindow class.
 * The AddCarWindow class provides a window to add a new car.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class AddCarWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	AddCarWindow(QWidget *parent = nullptr,
				 const std::string &ip = "",
				 const int port = 0,
				 const Account &acc = Account(),
				 QWidgetWithSubWindows *parentWindow = nullptr);
	~AddCarWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
	QLabel *carIdLabel, *carOwnerLabel, *carTypeLabel, *carColorLabel, *carYearLabel,
		*carImgPathLabel;
	QLineEdit *carIdLineEdit, *carOwnerLineEdit, *carTypeLineEdit, *carColorLineEdit,
		*carYearLineEdit, *carImgPathLineEdit;
	QPushButton *carImgSelectButton, *addCarButton;

  private slots:
	void getCarImagePath();
	void addCar();
};

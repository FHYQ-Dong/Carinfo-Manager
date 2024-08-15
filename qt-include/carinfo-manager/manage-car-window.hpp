/**
 * @file qt-include/carinfo-manager/manage-car-window.hpp
 * @brief Declaration of class ManageAccountWindow
 * 
 * @details
 * This file contains the declaration of the ManageAccountWindow class.
 * The ManageAccountWindow class provides a window to manage an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class ManageCarWindow : public QWidgetWithSubWindows {
	Q_OBJECT
  public:
	ManageCarWindow(QWidget *parent,
					const std::string &ip,
					const int port,
					const Account &acc,
					QWidgetWithSubWindows *parentWindow = nullptr);
	~ManageCarWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QPushButton *addCarButton, *updateCarButton, *removeCarButton;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;

  private slots:
	void addCar();
	void removeCar();
	void updateCar();
};

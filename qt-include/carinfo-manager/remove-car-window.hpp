/**
 * @file qt-include/carinfo-manager/remove-car-window.hpp
 * @brief Declaration of class RemoveCarWindow
 * 
 * @details
 * This file contains the declaration of the RemoveCarWindow class.
 * The RemoveCarWindow class provides a window to remove a car.
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
#include "carinfo-manager/httphandler-client.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class RemoveCarWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	RemoveCarWindow(QWidget *parent = nullptr,
					const std::string &ip = "",
					const int port = 0,
					const Account &acc = Account(),
					QWidgetWithSubWindows *parentWindow = nullptr);
	~RemoveCarWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
	QLabel *carIdLabel;
	QLineEdit *carIdLineEdit;
	QPushButton *removeCarButton;

  private slots:
	void removeCar();
};

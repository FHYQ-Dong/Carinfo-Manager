/**
 * @file qt-include/carinfo-manager/manage-window.hpp
 * @brief Declaration of class ManageWindow
 * 
 * @details
 * This file contains the declaration of the ManageWindow class.
 * The ManageWindow class provides a window to manage accounts and cars.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <set>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"
#include "carinfo-manager/search-car-window.hpp"

class ManageWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	ManageWindow(QWidget *parent = nullptr,
				 const std::string &ip = "",
				 const int port = 0,
				 const Account &acc = Account());
	~ManageWindow();

  private slots:
	void accountManage();
	void carManage();
	void accountSearch();
	void carSearch();
    void changePassword();
	void logout();

  private:
	QLabel *titleLabel;
	QLabel *usernameLabel, *accounttypeLabel;
	QPushButton *accountManageButton, *carManageButton, *accountSearchButton, *carSearchButton,
		*changePasswordButton, *logoutButton;
	std::string ip;
	int port;
	Account acc;
};

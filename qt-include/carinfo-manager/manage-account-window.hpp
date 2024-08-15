/**
 * @file qt-include/carinfo-manager/manage-account-window.hpp
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

class ManageAccountWindow : public QWidgetWithSubWindows {
	Q_OBJECT
  public:
	ManageAccountWindow(QWidget *parent,
						const std::string &ip,
						const int port,
						const Account &acc,
						QWidgetWithSubWindows *parentWindow = nullptr);
	~ManageAccountWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QPushButton *addAccountButton, *updateAccountButton, *removeAccountButton;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;

  private slots:
	void addAccount();
	void removeAccount();
	void updateAccount();
};

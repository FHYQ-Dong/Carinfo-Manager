/**
 * @file qt-include/carinfo-manager/update-account-window.hpp
 * @brief Declaration of class UpdateAccountWindow
 * 
 * @details
 * This file contains the declaration of the UpdateAccountWindow class.
 * The UpdateAccountWindow class provides a window to update an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class UpdateAccountWindow : public QWidgetWithSubWindows {
	Q_OBJECT
  public:
	UpdateAccountWindow(QWidget *parent,
						const std::string &ip,
						const int port,
						const Account &acc,
						QWidgetWithSubWindows *parentWindow = nullptr);
	~UpdateAccountWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
    QLabel *targetUsernameLabel, *newUsernameLabel, *newPasswordLabel, *confirmPasswordLabel, *newAccounttypeLabel;
	QLineEdit *targetUsernameEdit, *newUsernameEdit, *newPasswordEdit, *confirmPasswordEdit;
	QComboBox *newAccounttypeComboBox;
	QPushButton *updateAccountButton;

  private slots:
	void updateAccount();
};

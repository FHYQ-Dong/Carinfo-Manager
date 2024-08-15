/**
 * @file qt-include/carinfo-manager/search-account-window.hpp
 * @brief Declaration of class SearchAccountWindow
 * 
 * @details
 * This file contains the declaration of the SearchAccountWindow class.
 * The SearchAccountWindow class provides a window to search an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class SearchAccountWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	SearchAccountWindow(QWidget *parent = nullptr,
						const std::string &ip = "",
						const int port = 0,
						const Account &acc = Account(),
						QWidgetWithSubWindows *parentWindow = nullptr);
	~SearchAccountWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *titleLabel, *usernameLabel, *accounttypeLabel;
	QLabel *searchUsernameLabel;
	QLineEdit *searchUsernameLineEdit;
	QPushButton *searchButton, *saveAllAccountsButton;

  private slots:
	void searchAccount();
	void saveAllAccounts();
};

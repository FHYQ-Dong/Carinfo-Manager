/**
 * @file qt-include/carinfo-manager/login-window.hpp
 * @brief Declaration of class LoginWindow
 * 
 * @details
 * This file contains the declaration of the LoginWindow class.
 * The LoginWindow class provides a window to login.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>
#include <string>

class LoginWindow : public QWidget {
	Q_OBJECT

  public:
	LoginWindow(QWidget *parent = nullptr, const std::string &ip = "", const int port = 0);
	~LoginWindow();

  private slots:
	void login();
	void returnToSelectServer();

  private:
	std::string ip;
	int port;
	QLabel *titleLabel, *usernameLabel, *passwordLabel;
	QLineEdit *usernameEdit, *passwordEdit;
	QPushButton *loginButton, *returnButton;
	std::string username, password;
};

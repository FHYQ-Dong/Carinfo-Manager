/**
 * @file qt-include/carinfo-manager/select-server-window.hpp
 * @brief Declaration of class SelectServerWindow
 * 
 * @details
 * This file contains the declaration of the SelectServerWindow class.
 * The SelectServerWindow class provides a window to select a server.
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

class SelectServerWindow : public QWidget {
	Q_OBJECT

  public:
	SelectServerWindow(QWidget *parent = nullptr);
	~SelectServerWindow();

  private slots:
	void verifyServer();

  private:
	std::string ip;
	int port;
	QLabel *titleLabel, *serverLabel, *portLabel;
	QLineEdit *serverEdit, *portEdit;
	QPushButton *verifyButton;
};

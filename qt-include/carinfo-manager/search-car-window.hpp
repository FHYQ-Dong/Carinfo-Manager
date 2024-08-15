/**
 * @file qt-include/carinfo-manager/search-car-window.hpp
 * @brief Declaration of class SearchCarWindow
 * 
 * @details
 * This file contains the declaration of the SearchCarWindow class.
 * The SearchCarWindow class provides a window to search a car.
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
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class SearchCarWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	SearchCarWindow(QWidget *parent = nullptr,
					const std::string &ip = "",
					const int port = 0,
					const Account &acc = Account(),
					QWidgetWithSubWindows *parentWindow = nullptr);
	~SearchCarWindow();

  private slots:
	void search();

  private:
	std::string ip;
	int port;
	Account acc;
	QLabel *usernameLabel, *accounttypeLabel;
	QLabel *titleLabel, *caridLabel, *carownerLabel, *cartypeLabel, *carcolorLabel;
	QLineEdit *caridEdit, *carownerEdit, *cartypeEdit, *carcolorEdit;
	QPushButton *searchButton;
};

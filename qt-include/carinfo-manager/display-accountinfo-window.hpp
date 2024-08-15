/**
 * @file qt-include/carinfo-manager/display-accountinfo-window.hpp
 * @brief Declaration of class DisplayAccountinfoWindow
 * 
 * @details
 * This file contains the declaration of the DisplayAccountinfoWindow class.
 * The DisplayAccountinfoWindow class provides a window to display the information of an account.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class DisplayAccountinfoWindow : public QWidgetWithSubWindows {
	Q_OBJECT

  public:
	DisplayAccountinfoWindow(QWidget *parent = nullptr,
							 const AccountPool &accountpool = AccountPool(),
							 const std::string searchCarId = "",
							 const std::string &ip = "",
							 const int port = 0,
							 const Account &acc = Account(),
							 QWidgetWithSubWindows *parentWindow = nullptr);
	~DisplayAccountinfoWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	AccountPool accountpool;
	std::string searchCarId;
	QVBoxLayout *outerLayout, *innerLayout;
	QWidget *accountinfoWidget;
	QScrollArea *accountinfoScrollArea;
	QLabel *accountQueryLabel;
};

/**
 * @file display-carinfo-window.hpp
 * @brief Declaration of class DisplayCarInfoWindow
 * 
 * @details
 * This file contains the declaration of the DisplayCarInfoWindow class.
 * The DisplayCarInfoWindow class provides a window to display the information of a car.
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
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/qwidget-with-subwindows.hpp"

class SearchCarQuery {
  public:
	SearchCarQuery(const std::string &searchCarId = "",
				   const std::string &searchCarOwner = "",
				   const std::string &searchCarType = "",
				   const std::string &searchCarColor = "");
	SearchCarQuery(const SearchCarQuery &query);
	~SearchCarQuery();

  public:
	std::string searchCarId, searchCarOwner, searchCarType, searchCarColor;
};

class DisplayCarInfoWindow : public QWidgetWithSubWindows, private SearchCarQuery {
	Q_OBJECT

  public:
	DisplayCarInfoWindow(QWidget *parent = nullptr,
						 const CarPool &carpool = CarPool(),
						 const SearchCarQuery &query = SearchCarQuery(),
						 const std::string &ip = "",
						 const int port = 0,
						 const Account &acc = Account(),
						 QWidgetWithSubWindows *parentWindow = nullptr);
	~DisplayCarInfoWindow();

  private:
	std::string ip;
	int port;
	Account acc;
	CarPool carpool;
	QVBoxLayout *outerLayout, *innerLayout;
	QWidget *carinfoWidget;
	QScrollArea *carinfoScrollArea;
	QLabel *carQueryTitleLabel, *carQueryIdLabel, *carQueryOwnerLabel, *carQueryTypeLabel,
		*carQueryColorLabel;
};

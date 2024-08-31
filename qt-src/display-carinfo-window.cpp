/**
 * @file qt-src/display-carinfo-window.cpp
 * @brief Implementation of class DisplayCarInfoWindow
 * 
 * @details
 * This file contains the implementation of the DisplayCarInfoWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/display-carinfo-window.hpp"
#include <QByteArray>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QString>
#include <sstream>
#include <string>
#include "carinfo-manager/httphandler-client.hpp"
#include "json/json.hpp"

SearchCarQuery::SearchCarQuery(const std::string &searchCarId,
							   const std::string &searchCarOwner,
							   const std::string &searchCarType,
							   const std::string &searchCarColor) {
	this->searchCarId = searchCarId;
	this->searchCarOwner = searchCarOwner;
	this->searchCarType = searchCarType;
	this->searchCarColor = searchCarColor;
}

SearchCarQuery::SearchCarQuery(const SearchCarQuery &query) {
	this->searchCarId = query.searchCarId;
	this->searchCarOwner = query.searchCarOwner;
	this->searchCarType = query.searchCarType;
	this->searchCarColor = query.searchCarColor;
}

SearchCarQuery::~SearchCarQuery() {}

DisplayCarInfoWindow::DisplayCarInfoWindow(QWidget *parent,
										   const CarPool &carpool,
										   const SearchCarQuery &query,
										   const std::string &ip,
										   const int port,
										   const Account &acc,
										   QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow),
	  SearchCarQuery(query),
	  ip(ip),
	  port(port),
	  acc(acc),
	  carpool(carpool) {
	this->setParent(parent);
	this->setWindowTitle("车辆信息");
	this->setFixedSize(800, 600);

	outerLayout = new QVBoxLayout(this);
	outerLayout->setContentsMargins(0, 0, 0, 0);

	carinfoScrollArea = new QScrollArea(this);
	carinfoScrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	carinfoScrollArea->setWidgetResizable(true);
	outerLayout->addWidget(carinfoScrollArea);

	carinfoWidget = new QWidget(this);
	carinfoScrollArea->setWidget(carinfoWidget);

	innerLayout = new QVBoxLayout(carinfoWidget);
	innerLayout->addSpacing(10);

	carQueryTitleLabel = new QLabel(QString::fromStdString("查询条件："), carinfoWidget);
	QFont font = carQueryTitleLabel->font();
	font.setBold(true);
	carQueryTitleLabel->setFont(font);
	carQueryTitleLabel->setAlignment(Qt::AlignCenter);
	innerLayout->addWidget(carQueryTitleLabel);

	if (!searchCarId.empty()) {
		carQueryIdLabel =
			new QLabel(QString::fromStdString("车牌号：\u3000" + searchCarId), carinfoWidget);
		carQueryIdLabel->setAlignment(Qt::AlignCenter);
		carQueryIdLabel->setFont(font);
		innerLayout->addWidget(carQueryIdLabel);
	}
	if (!searchCarOwner.empty()) {
		carQueryOwnerLabel =
			new QLabel(QString::fromStdString("车主：\u3000" + searchCarOwner), carinfoWidget);
		carQueryOwnerLabel->setAlignment(Qt::AlignCenter);
		carQueryOwnerLabel->setFont(font);
		innerLayout->addWidget(carQueryOwnerLabel);
	}
	if (!searchCarType.empty()) {
		carQueryTypeLabel =
			new QLabel(QString::fromStdString("车型：\u3000" + searchCarType), carinfoWidget);
		carQueryTypeLabel->setAlignment(Qt::AlignCenter);
		carQueryTypeLabel->setFont(font);
		innerLayout->addWidget(carQueryTypeLabel);
	}
	if (!searchCarColor.empty()) {
		carQueryColorLabel =
			new QLabel(QString::fromStdString("颜色：\u3000" + searchCarColor), carinfoWidget);
		carQueryColorLabel->setAlignment(Qt::AlignCenter);
		carQueryColorLabel->setFont(font);
		innerLayout->addWidget(carQueryColorLabel);
	}

	QLabel *placeholderLabel = new QLabel("", carinfoWidget);
	placeholderLabel->setFixedSize(800, 10);
	innerLayout->addWidget(placeholderLabel);

	auto car_list = carpool.list();
	for (auto it = car_list.begin(); it != car_list.end(); ++it) {
		Car car = *it;

		QWidget *itemWidget = new QWidget(carinfoWidget);
		QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
		itemLayout->setContentsMargins(5, 5, 5, 5);

		QWidget *itemTextWidget = new QWidget(itemWidget), *itemPicWidget = new QWidget(itemWidget);
		itemLayout->addWidget(itemTextWidget);
		itemLayout->addWidget(itemPicWidget);

		QVBoxLayout *itemTextLayout = new QVBoxLayout(itemTextWidget),
					*itemPicLayout = new QVBoxLayout(itemPicWidget);
		itemTextLayout->setSpacing(10);
		itemPicLayout->setContentsMargins(0, 0, 0, 0);

		QLabel *itemIdLabel = new QLabel(QString::fromStdString("车牌号：\u3000" + car.getId()),
										 itemTextWidget),
			   *itemOwnerLabel = new QLabel(QString::fromStdString("车主：\u3000" + car.getOwner()),
											itemTextWidget),
			   *itemTypeLabel = new QLabel(QString::fromStdString("车型：\u3000" + car.getType()),
										   itemTextWidget),
			   *itemColorLabel = new QLabel(QString::fromStdString("颜色：\u3000" + car.getColor()),
											itemTextWidget),
			   *itemPicLabel = new QLabel(itemPicWidget);
		itemIdLabel->setAlignment(Qt::AlignCenter);
		itemColorLabel->setAlignment(Qt::AlignCenter);
		itemTypeLabel->setAlignment(Qt::AlignCenter);
		itemOwnerLabel->setAlignment(Qt::AlignCenter);
		itemIdLabel->resize(350, 30);
		itemColorLabel->resize(350, 30);
		itemTypeLabel->resize(350, 30);
		itemOwnerLabel->resize(350, 30);
		itemPicLabel->resize(360, 280);

		auto imgRes = ClientHttpHandler::handler_get_carimg(ip, port, acc, car.getImagePath());
		if (!imgRes) {
			QMessageBox::critical(this,
								  "错误",
								  std::string("获取车辆图片失败\n车牌号：" + car.getId() +
											  "\n错误信息：" + imgRes.message)
									  .c_str());
			continue;
		}
		QByteArray imgData = QByteArray::fromStdString(imgRes.message);
		QPixmap img = QPixmap::fromImage(QImage::fromData(imgData));

		itemPicLabel->setPixmap(img.scaled(360, 280, Qt::KeepAspectRatio));
		itemPicLabel->setAlignment(Qt::AlignCenter);
		itemTextLayout->addStretch();
		itemTextLayout->addWidget(itemIdLabel, 0, Qt::AlignCenter);
		itemTextLayout->addWidget(itemColorLabel, 0, Qt::AlignCenter);
		itemTextLayout->addWidget(itemTypeLabel, 0, Qt::AlignCenter);
		itemTextLayout->addWidget(itemOwnerLabel, 0, Qt::AlignCenter);
		itemTextLayout->addStretch();
		itemPicLayout->addWidget(itemPicLabel);
		innerLayout->addWidget(itemWidget);

		if (it != car_list.end() - 1) {
			QFrame *line = new QFrame(carinfoWidget);
			line->setFrameShape(QFrame::HLine);
			line->setFrameShadow(QFrame::Sunken);
			innerLayout->addWidget(line);
		}
	}
}

DisplayCarInfoWindow::~DisplayCarInfoWindow() {
	closeAllSubWindows();
	removeFromParent();
}

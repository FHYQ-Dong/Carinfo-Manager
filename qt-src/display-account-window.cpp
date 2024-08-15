/**
 * @file qt-src/display-accountinfo-window.cpp
 * @brief Implementation of class DisplayAccountinfoWindow
 * 
 * @details
 * This file contains the implementation of the DisplayAccountinfoWindow class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include <QFont>
#include <QString>
#include "carinfo-manager/display-accountinfo-window.hpp"
#include "carinfo-manager/httphandler-client.hpp"

DisplayAccountinfoWindow::DisplayAccountinfoWindow(QWidget *parent,
												   const AccountPool &accountpool,
												   const std::string searchCarId,
												   const std::string &ip,
												   const int port,
												   const Account &acc,
												   QWidgetWithSubWindows *parentWindow)
	: QWidgetWithSubWindows(parent, parentWindow),
	  ip(ip),
	  port(port),
	  acc(acc),
	  accountpool(accountpool),
	  searchCarId(searchCarId) {
	this->setParent(parent);
	this->setWindowTitle("账户信息");
	this->setFixedSize(300, 300);

	outerLayout = new QVBoxLayout(this);
	outerLayout->setContentsMargins(0, 0, 0, 0);

	accountinfoScrollArea = new QScrollArea(this);
	accountinfoScrollArea->setWidgetResizable(true);
	outerLayout->addWidget(accountinfoScrollArea);

	accountinfoWidget = new QWidget(this);
	accountinfoScrollArea->setWidget(accountinfoWidget);

	innerLayout = new QVBoxLayout(accountinfoWidget);

	accountQueryLabel =
		new QLabel(std::string("查询条件：\u3000" + searchCarId).c_str(), accountinfoWidget);
	accountQueryLabel->setAlignment(Qt::AlignCenter);
	QFont font = accountQueryLabel->font();
	font.setBold(true);
	accountQueryLabel->setFont(font);
	innerLayout->addWidget(accountQueryLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

	QLabel *placeholderLabel = new QLabel(this);
	placeholderLabel->setFixedSize(300, 10);
	innerLayout->addWidget(placeholderLabel, 0, Qt::AlignHCenter | Qt::AlignTop);

	auto acc_list = accountpool.list();
	for (auto it = acc_list.begin(); it != acc_list.end(); ++it) {
		Account account = *it;

		QWidget *itemWidget = new QWidget(accountinfoWidget);
		QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);
		itemLayout->setContentsMargins(5, 5, 5, 5);
		itemLayout->setSpacing(10);

		QLabel *itemUsernameLabel = new QLabel(
				   QString::fromStdString("账号：\u3000" + account.getUsername()), itemWidget),
			   *itemPasswdHashTitleLabel = new QLabel("密码哈希：", itemWidget),
			   *itemPasswdHashLabel_1 = new QLabel(
				   QString::fromStdString(account.getPasswdHash().substr(0, 16)), itemWidget),
			   *itemPasswdHashLabel_2 = new QLabel(
				   QString::fromStdString(account.getPasswdHash().substr(16, 16)), itemWidget),
			   *itemPasswdHashLabel_3 = new QLabel(
				   QString::fromStdString(account.getPasswdHash().substr(32, 16)), itemWidget),
			   *itemPasswdHashLabel_4 = new QLabel(
				   QString::fromStdString(account.getPasswdHash().substr(48, 16)), itemWidget),
			   *itemAccountTypeLabel = new QLabel(
				   QString::fromStdString("账户类型：\u3000" +
										  std::string((account.isAdmin() ? "管理员" : "普通用户"))),
				   itemWidget);
		itemUsernameLabel->setAlignment(Qt::AlignCenter);
		itemPasswdHashTitleLabel->setAlignment(Qt::AlignCenter);
		itemPasswdHashLabel_1->setAlignment(Qt::AlignCenter);
		itemPasswdHashLabel_2->setAlignment(Qt::AlignCenter);
		itemPasswdHashLabel_3->setAlignment(Qt::AlignCenter);
		itemPasswdHashLabel_4->setAlignment(Qt::AlignCenter);
		itemAccountTypeLabel->setAlignment(Qt::AlignCenter);
		itemUsernameLabel->resize(250, 30);
		itemPasswdHashTitleLabel->resize(250, 30);
		itemPasswdHashLabel_1->resize(250, 30);
		itemPasswdHashLabel_2->resize(250, 30);
		itemPasswdHashLabel_3->resize(250, 30);
		itemPasswdHashLabel_4->resize(250, 30);
		itemAccountTypeLabel->resize(250, 30);
		itemLayout->addWidget(itemUsernameLabel, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemPasswdHashTitleLabel, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemPasswdHashLabel_1, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemPasswdHashLabel_2, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemPasswdHashLabel_3, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemPasswdHashLabel_4, 0, Qt::AlignHCenter);
		itemLayout->addWidget(itemAccountTypeLabel, 0, Qt::AlignHCenter);

		innerLayout->addWidget(itemWidget, 0, Qt::AlignHCenter | Qt::AlignTop);

		if (it != acc_list.end() - 1) {
			QFrame *line = new QFrame(accountinfoWidget);
			line->setFrameShape(QFrame::HLine);
			line->setFrameShadow(QFrame::Sunken);
			innerLayout->addWidget(line);
		}
	}
}

DisplayAccountinfoWindow::~DisplayAccountinfoWindow() {
	closeAllSubWindows();
	removeFromParent();
}

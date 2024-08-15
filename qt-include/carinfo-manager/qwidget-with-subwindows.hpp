/**
 * @file qt-include/carinfo-manager/qwidget-with-subwindows.hpp
 * @brief Declaration of class QWidgetWithSubWindows
 * 
 * @details
 * This file contains the declaration of the QWidgetWithSubWindows class.
 * The QWidgetWithSubWindows class provides a window with sub windows.
 * It can be used as a base class for windows, which close all sub windows when needed.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#include <QWidget>
#include <set>

class QWidgetWithSubWindows : public QWidget {
	Q_OBJECT

  protected:
	std::set<QWidgetWithSubWindows *> subWindows;
	QWidgetWithSubWindows *parentWindow;
	bool closeFromParent = false;

  public:
	QWidgetWithSubWindows(
		QWidget *parent,
		QWidgetWithSubWindows *parentWindow = nullptr,
		const std::set<QWidgetWithSubWindows *> &subWindows = std::set<QWidgetWithSubWindows *>());
	void addSubWindow(QWidgetWithSubWindows *window);
	void removeSubWindow(QWidgetWithSubWindows *window);
	void closeSubWindow(QWidgetWithSubWindows *window);
	void closeAllSubWindows();
	void removeFromParent();
	void setCloseFromParent(const bool closeFromParent);
	bool getCloseFromParent() const;
	virtual void closeEvent(QCloseEvent *event) override;
	~QWidgetWithSubWindows();
};

/**
 * @file qt-src/qwidget-with-subwindows.cpp
 * @brief Implementation of class QWidgetWith
 * 
 * @details
 * This file contains the implementation of the QWidgetWithSubWindows class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/qwidget-with-subwindows.hpp"
#include <QCloseEvent>

QWidgetWithSubWindows::QWidgetWithSubWindows(QWidget *parent,
											 QWidgetWithSubWindows *parentWindow,
											 const std::set<QWidgetWithSubWindows *> &subWindows)
	: QWidget(parent), parentWindow(parentWindow), subWindows(subWindows) {}

void QWidgetWithSubWindows::addSubWindow(QWidgetWithSubWindows *window) {
	if (subWindows.find(window) == subWindows.end())
		subWindows.insert(window);
}

void QWidgetWithSubWindows::removeSubWindow(QWidgetWithSubWindows *window) {
	if (subWindows.find(window) != subWindows.end())
		subWindows.erase(window);
}

void QWidgetWithSubWindows::closeSubWindow(QWidgetWithSubWindows *window) {
	if (subWindows.find(window) != subWindows.end()) {
		window->setCloseFromParent(true);
		window->close();
		delete window;
		subWindows.erase(window);
	}
}

void QWidgetWithSubWindows::closeAllSubWindows() {
	for (QWidgetWithSubWindows *window : subWindows) {
		window->setCloseFromParent(true);
		window->close();
		delete window;
	}
	subWindows.clear();
}

void QWidgetWithSubWindows::removeFromParent() {
	if (parentWindow != nullptr)
		parentWindow->removeSubWindow(this);
}

QWidgetWithSubWindows::~QWidgetWithSubWindows() {
	closeAllSubWindows();
}

void QWidgetWithSubWindows::setCloseFromParent(const bool closeFromParent) {
	this->closeFromParent = closeFromParent;
}

bool QWidgetWithSubWindows::getCloseFromParent() const {
	return closeFromParent;
}

void QWidgetWithSubWindows::closeEvent(QCloseEvent *event) {
	if (!closeFromParent)
		removeFromParent();
	closeAllSubWindows();
	event->accept();
}

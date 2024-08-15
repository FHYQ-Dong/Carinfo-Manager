/**
 * @file qt-src/client-window.cpp
 * @brief Implementation of class ClientMain
 * 
 * @details
 * This file contains the implementation of the ClientMain class.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/client-window.hpp"
#include <locale>
#include "carinfo-manager/select-server-window.hpp"

ClientMain::ClientMain(int argc, char *argv[]) : QApplication(argc, argv) {}

ClientMain::~ClientMain() {}

int ClientMain::run() {
	SelectServerWindow *selectServerWindow = new SelectServerWindow();
	selectServerWindow->show();
	return exec();
}

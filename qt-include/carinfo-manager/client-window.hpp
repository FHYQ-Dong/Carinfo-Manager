/**
 * @file qt-include/carinfo-manager/client-window.hpp
 * @brief Declaration of class ClientMain
 * 
 * @details
 * This file contains the declaration of the ClientMain class.
 * The ClientMain class provides the main entry of the client program.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <QApplication>

class ClientMain : public QApplication {
  public:
	ClientMain(int argc, char *argv[]);
	~ClientMain();
	int run();
};

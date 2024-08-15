/**
 * @file src/client-main.cpp
 * @brief Main entry of the client program
 * 
 * @details
 * This file contains the main entry of the client program.
 * The main function starts the logging system and the client program.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include <QApplication>
#include "carinfo-manager/client-window.hpp"
#include "carinfo-manager/log.hpp"

int main(int argc, char *argv[]) {
	// start logging
	MyLogger::registerLogger("carinfo-manager-logger",
							 MyLogger::LOG_LEVEL::DEBUG,
							 MyLogger::LOG_TYPE::CONSOLE | MyLogger::LOG_TYPE::FILE,
							 "log/carinfo-manager-client.log");

	// start client
	ClientMain clientMain(argc, argv);
	return clientMain.run();
}

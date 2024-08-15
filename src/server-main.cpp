/**
 * @file src/server-main.cpp
 * @brief Implementation of a server for the carinfo-manager project using httplib library
 * 
 * @details
 * This file contains the main entry of the server program.
 * The main function starts the logging system, loads config, data, and starts the server.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/httphandler-server.hpp"
#include "carinfo-manager/log.hpp"
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <config_json_file_path>" << endl;
		return 1;
	}

	// start logging
	MyLogger::registerLogger("carinfo-manager-logger",
							 MyLogger::LOG_LEVEL::DEBUG,
							 MyLogger::LOG_TYPE::CONSOLE | MyLogger::LOG_TYPE::FILE,
							 "log/carinfo-manager-server.log");

	// load config
	ifstream config_file(argv[1]);
	if (!config_file.is_open()) {
		MyLogger::log(
			"carinfo-manager-logger", MyLogger::LOG_LEVEL::ERROR, "Cannot open config file");
		return 1;
	}
	json config_json_obj = json::parse(config_file);
	config_file.close();
	if (!config_json_obj.is_object()) {
		MyLogger::log("carinfo-manager-logger", MyLogger::LOG_LEVEL::ERROR, "Invalid config file");
		return 1;
	}
	if (config_json_obj.find("dataDir") == config_json_obj.end() ||
		!config_json_obj["dataDir"].is_string() ||
		config_json_obj.find("ip") == config_json_obj.end() || !config_json_obj["ip"].is_string() ||
		config_json_obj.find("port") == config_json_obj.end() ||
		!config_json_obj["port"].is_number_unsigned()) {
		MyLogger::log("carinfo-manager-logger", MyLogger::LOG_LEVEL::ERROR, "Invalid config file");
		return 1;
	}
	string dataDir = string(config_json_obj["dataDir"]);
	string ip = string(config_json_obj["ip"]);
	int port = int(config_json_obj["port"]);

	// print config
	MyLogger::log(
		"carinfo-manager-logger",
		MyLogger::LOG_LEVEL::INFO,
		"Using config:\n- dataDir: " + dataDir + "\n- ip: " + ip + "\n- port: " + to_string(port));

	// load data
	AccountPool accountpool;
	CarPool carpool;
	ifstream account_file(dataDir + "account.json");
	if (accountpool.load(account_file) != 0) {
		MyLogger::log("carinfo-manager-logger", MyLogger::LOG_LEVEL::ERROR, "Cannot load account data");
		return 1;
	}
	account_file.close();
	ifstream car_file(dataDir + "car.json");
	if (carpool.load(car_file) != 0) {
		MyLogger::log("carinfo-manager-logger", MyLogger::LOG_LEVEL::ERROR, "Cannot load car data");
		return 1;
	}
	car_file.close();

	// modify img files
	set<string> imgFiles;
	for (auto &car : carpool.list())
		imgFiles.insert(car.getImagePath());
	for (auto &file : filesystem::directory_iterator(dataDir + "img/")) {
		if (file.is_regular_file()) {
			string fullPath = file.path().string();
			if (imgFiles.find(fullPath) == imgFiles.end())
				filesystem::remove(fullPath);
		}
	}

	// config server
	httplib::Server svr;
	ServerHttpHandler handler(accountpool, carpool, dataDir + "img/");
	svr.Get("/test_connection", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_test_connection(req, res);
	});
	svr.Post("/test_connection", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_test_connection(req, res);
	});
	svr.Post("/login", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_login(req, res);
	});
	svr.Post("/change_password", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_change_password(req, res);
		ofstream account_file(dataDir + "account.json");
		accountpool.save(account_file);
		account_file.close();
	});
	svr.Post("/get_carinfo", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_get_carinfo(req, res);
	});
	svr.Post("/get_carimg", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_get_carimg(req, res);
	});
	svr.Post("/add_car", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_add_car(req, res);
		ofstream car_file(dataDir + "car.json");
		carpool.save(car_file);
		car_file.close();
	});
	svr.Post("/remove_car", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_remove_car(req, res);
		ofstream car_file(dataDir + "car.json");
		carpool.save(car_file);
		car_file.close();
	});
	svr.Post("/update_car", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_update_car(req, res);
		ofstream car_file(dataDir + "car.json");
		carpool.save(car_file);
		car_file.close();
	});
	svr.Post("/get_accountinfo", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_get_accountinfo(req, res);
	});
	svr.Post("/get_all_account", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_get_all_account(req, res);
	});
	svr.Post("/add_account", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_add_account(req, res);
		ofstream account_file(dataDir + "account.json");
		accountpool.save(account_file);
		account_file.close();
	});
	svr.Post("/remove_account", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_remove_account(req, res);
		ofstream account_file(dataDir + "account.json");
		accountpool.save(account_file);
		account_file.close();
	});
	svr.Post("/update_account", [&](const httplib::Request &req, httplib::Response &res) {
		handler.handler_update_account(req, res);
		ofstream account_file(dataDir + "account.json");
		accountpool.save(account_file);
		account_file.close();
	});

	// start server
	MyLogger::log("carinfo-manager-logger", MyLogger::LOG_LEVEL::INFO, "Server started");
	svr.listen(ip.c_str(), port);
	return 0;
}

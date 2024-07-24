/**
 * @file src/server.cpp
 * @brief Implementation of a server for the carinfo-manager project using httplib library
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/httphandler.hpp"
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using json = nlohmann::json;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <config_json_file_path>" << endl;
        return 1;
    }

    // load config
    ifstream config_file(argv[1]);
    if (!config_file.is_open()) {
        cout << "Error: Cannot open config file" << endl;
        return 1;
    }
    json config_json_obj = json::parse(config_file);
    config_file.close();
    if (!config_json_obj.is_object()) {
        cout << "Error: Invalid config file" << endl;
        return 1;
    }
    if (config_json_obj.find("dataDir") == config_json_obj.end() || !config_json_obj["dataDir"].is_string() \
        || config_json_obj.find("ip") == config_json_obj.end() || !config_json_obj["ip"].is_string() \
        || config_json_obj.find("port") == config_json_obj.end() || !config_json_obj["port"].is_number_unsigned()
    ) {
        cout << "Error: Invalid config file" << endl;
        return 1;
    }
    string dataDir = string(config_json_obj["dataDir"]);
    string ip = string(config_json_obj["ip"]);
    int port = int(config_json_obj["port"]);

    // print config
    cout << "Using config:" << endl;
    cout << "- dataDir: " << dataDir << endl;
    cout << "- ip: " << ip << endl;
    cout << "- port: " << port << endl;

    // load data
    AccountPool accountpool;
    CarPool carpool;
    ifstream account_file(dataDir + "account.json");
    if (accountpool.load(account_file) != 0) {
        cout << "Error: Cannot load account data" << endl;
        return 1;
    }
    account_file.close();
    ifstream car_file(dataDir + "car.json");
    if (carpool.load(car_file) != 0) {
        cout << "Error: Cannot load car data" << endl;
        return 1;
    }
    car_file.close();

    // config server
    httplib::Server svr;
    HttpHandler handler(accountpool, carpool, dataDir + "img/");
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
    cout << "Server started at " << ip << ":" << port << endl;
    svr.listen(ip.c_str(), port);
    return 0;
}

/**
 * @file include/carinfo-manager/httphandler-server.hpp
 * @brief Declaration of class ServerHttpHandler
 * 
 * @details
 * This file contains the declaration of the ServerHttpHandler class, which handles HTTP requests for car information management.
 * The ServerHttpHandler class provides methods for handling various types of requests, such as testing the connection, login or password change, car management, and account management.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"

class ServerHttpHandler {
  private:
	AccountPool &accountpool;
	CarPool &carpool;
	std::string imgDir;  // end with '/'

  private:
	nlohmann::json parse_post_body(const httplib::Request &req) const;

  public:
	ServerHttpHandler(AccountPool &accountpool, CarPool &carpool, std::string imgDir);
	// test connection
	void handler_test_connection(const httplib::Request &req, httplib::Response &res) const;
	// login or change password
	void handler_login(const httplib::Request &req, httplib::Response &res) const;
	void handler_change_password(const httplib::Request &req, httplib::Response &res);
	// car management
	void handler_get_carinfo(const httplib::Request &req, httplib::Response &res) const;
	void handler_get_carimg(const httplib::Request &req, httplib::Response &res) const;
	void handler_add_car(const httplib::Request &req, httplib::Response &res);
	void handler_remove_car(const httplib::Request &req, httplib::Response &res);
	void handler_update_car(const httplib::Request &req, httplib::Response &res);
	// account management
	void handler_get_accountinfo(const httplib::Request &req, httplib::Response &res) const;
	void handler_get_all_account(const httplib::Request &req, httplib::Response &res) const;
	void handler_add_account(const httplib::Request &req, httplib::Response &res);
	void handler_remove_account(const httplib::Request &req, httplib::Response &res);
	void handler_update_account(const httplib::Request &req, httplib::Response &res);
};

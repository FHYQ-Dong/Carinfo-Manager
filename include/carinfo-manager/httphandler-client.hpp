/**
 * @file include/carinfo-manager/httphandler-client.hpp
 * @brief Declaration of ClientHttpHandler class, AccountVerifyResult enum, and HttpResult class
 * 
 * @details
 * This file contains the declaration of the ClientHttpHandler class, AccountVerifyResult enum, and HttpResult class.
 * The ClientHttpHandler class provides static methods to handle HTTP requests from the client side.
 * The AccountVerifyResult enum represents the result of account verification.
 * The HttpResult class represents the result of an HTTP request.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <string>
#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"

class ClientHttpHandler {
  public:
	enum class AccountVerifyResult {
		SUCCESS = 0,
		ACCOUNT_NOT_FOUND_OR_WRONG_PASSWORD = 1,
		NETWORK_ERROR = 2,
		INVALID_RESPONSE = 3,
		BAD_REQUEST = 4
	};

	class HttpResult {
	  public:
		int status;
		std::string message;
		nlohmann::json json_obj;

		HttpResult(int status = 0,
				   const std::string &message = "",
				   const nlohmann::json &json_obj = nlohmann::json())
			: status(status), message(message), json_obj(json_obj) {}

		~HttpResult() {}

		operator bool() const { return status == 200; }
	};

  private:
	static nlohmann::json parse_resp_content(const httplib::Response &resp);

  public:
	ClientHttpHandler();

	static bool handler_verify_server(const std::string &ip, int port);
	static AccountVerifyResult handler_login(const std::string &ip, int port, Account &acc);
	static HttpResult handler_get_carinfo(const std::string &ip,
									   int port,
									   const Account &acc,
									   const std::string &carid,
									   const std::string &carowner,
									   const std::string &carcolor,
									   const std::string &cartype);
	static HttpResult handler_get_carimg(const std::string &ip,
										  int port,
										  const Account &acc,
										  std::string car_img_path);
	static HttpResult handler_add_car(const std::string &ip,
									  const int port,
									  const Account &acc,
									  const std::string &car_id,
									  const std::string &car_owner,
									  const std::string &car_type,
									  const std::string &car_color,
									  const int car_year,
									  const std::string &car_img,
									  const std::string &car_img_type);
	static HttpResult handler_remove_car(const std::string &ip,
										 const int port,
										 const Account &acc,
										 const std::string &car_id);
	static HttpResult handler_update_car(const std::string &ip,
										 const int port,
										 const Account &acc,
										 const std::string &original_car_id,
										 const std::string &new_car_id,
										 const std::string &new_car_owner,
										 const std::string &new_car_type,
										 const std::string &new_car_color,
										 const int new_car_year,
										 const std::string &new_car_img,
										 const std::string &new_car_img_type);
	static HttpResult handler_search_account(const std::string &ip,
											 const int port,
											 const Account &acc,
											 const std::string &search_username);
	static HttpResult handler_get_all_accounts(const std::string &ip,
											   const int port,
											   const Account &acc);
	static HttpResult handler_add_account(const std::string &ip,
										  const int port,
										  const Account &acc,
										  const std::string &new_username,
										  const std::string &new_passwd_hash,
										  const Account::AccountType new_account_type);
	static HttpResult handler_remove_account(const std::string &ip,
											 const int port,
											 const Account &acc,
											 const std::string &target_username);
	static HttpResult handler_update_account(const std::string &ip,
											 const int port,
											 const Account &acc,
											 const std::string &target_username,
											 const std::string &new_username,
											 const std::string &new_passwd_hash,
											 const Account::AccountType new_account_type);
    static HttpResult handler_change_password(const std::string &ip,
                                             const int port,
                                             const Account &acc,
                                             const std::string &new_passwd_hash);
};

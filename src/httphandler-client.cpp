/** 
 * @file src/httphandler-client.cpp
 * @brief Implementation of class ClientHttpHandler
 * 
 * @details
 * This file contains the implementation of the ClientHttpHandler class.
 * The ClientHttpHandler class is used to handle HTTP requests from the client side.
 * It sends HTTP requests to the server and receives responses.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/httphandler-client.hpp"
#include <sstream>
#include "carinfo-manager/log.hpp"
using json = nlohmann::json;

ClientHttpHandler::ClientHttpHandler() {}

/**
 * @brief Parse the response content of a HTTP response
 * 
 * @param resp The HTTP response
 * @return json The parsed JSON object of the response content
 */
json ClientHttpHandler::parse_resp_content(const httplib::Response &resp) {
	json resp_json_obj;
	try {
		resp_json_obj = json::parse(resp.body);
	}
	catch (json::parse_error &e) {
		resp_json_obj["error"] = "Invalid response content";
	}
	return resp_json_obj;
}

/**
 * @brief Verify the connection to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * 
 * @return bool True if the connection is successful, false otherwise
 */
bool ClientHttpHandler::handler_verify_server(const std::string &ip, int port) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);
	httplib::Result res = client.Get("/test_connection");
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Verify Server] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return false;
	}
	httplib::Response resp = res.value();
	if (resp.status == 200 && resp.body == "Connection is OK") {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Verify Server] Connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
		return true;
	}
	else {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Verify Server] Failed to connect to " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
		return false;
	}
}

/**
 * @brief Verify the account by sending a login request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * 
 * @return AccountVerifyResult The result of the account verification
 */
ClientHttpHandler::AccountVerifyResult ClientHttpHandler::handler_login(const std::string &ip,
																		int port,
																		Account &acc) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()}};
	httplib::Result res = client.Post("/login", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Login] Failed to connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return AccountVerifyResult::NETWORK_ERROR;
	}
	httplib::Response resp = res.value();
	if (resp.status == 200) {
		json resp_json_obj = parse_resp_content(resp);
		if (resp_json_obj.find("error") != resp_json_obj.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::ERROR,
						  "[HTTP Login] Failed to login to " + ip + ":" + std::to_string(port) +
							  ". \n- Status: Invalid Response. \n- Error: " +
							  resp_json_obj["error"].get<std::string>());
			return AccountVerifyResult::INVALID_RESPONSE;
		}
		else
			acc.setAccountType(Account::AccountType(resp_json_obj["account_type"].get<int>()));
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Login] Login to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
		return AccountVerifyResult::SUCCESS;
	}
	else if (resp.status == 401 && resp.body == "Account Not Found or Wrong Password") {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Login] Failed to login to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
		return AccountVerifyResult::ACCOUNT_NOT_FOUND_OR_WRONG_PASSWORD;
	}
	else if (resp.status == 400 && resp.body == "Bad Request") {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Login] Failed to login to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
		return AccountVerifyResult::BAD_REQUEST;
	}
	else {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Login] Failed to login to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
		return AccountVerifyResult::INVALID_RESPONSE;
	}
}

/**
 * @brief Get the information of a car by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param car_id The ID of the car
 * @param car_owner The owner of the car
 * @param car_color The color of the car
 * @param car_type The type of the car
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_get_carinfo(const std::string &ip,
																	 int port,
																	 const Account &acc,
																	 const std::string &car_id,
																	 const std::string &car_owner,
																	 const std::string &car_color,
																	 const std::string &car_type) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"car_id", car_id},
											 {"car_owner", car_owner},
											 {"car_color", car_color},
											 {"car_type", car_type}};
	httplib::Result res = client.Post("/get_carinfo", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get Car Info] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200) {
		json resp_json_obj = parse_resp_content(resp);
		if (resp_json_obj.find("error") != resp_json_obj.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::ERROR,
						  "[HTTP Get Car Info] Failed to get car info from " + ip + ":" +
							  std::to_string(port) + ". \n- Status: Invalid Response. \n- Error: " +
							  resp_json_obj["error"].get<std::string>());
			return HttpResult(0, resp_json_obj["error"].get<std::string>());
		}
		else {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::DEBUG,
						  "[HTTP Get Car Info] Get car info from " + ip + ":" +
							  std::to_string(port) + ". \n- Status Code: " +
							  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
			return HttpResult(resp.status, resp.body, resp_json_obj);
		}
	}
	else {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get Car Info] Failed to get car info from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
		return HttpResult(resp.status, resp.body);
	}
}

/**
 * @brief Get the image of a car by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param car_img_path The path of the car image
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_get_carimg(const std::string &ip,
																	int port,
																	const Account &acc,
																	std::string car_img_path) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"car_img_path", car_img_path}};
	httplib::Result res = client.Post("/get_carimg", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get Car Image] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Get Car Image] Get car image from " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status));
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get Car Image] Failed to get car image from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Add a car by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param car_id The ID of the car
 * @param car_owner The owner of the car
 * @param car_type The type of the car
 * @param car_color The color of the car
 * @param car_year The year of the car
 * @param car_img The image of the car
 * @param car_img_type The type of the image of the car
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_add_car(const std::string &ip,
																 const int port,
																 const Account &acc,
																 const std::string &car_id,
																 const std::string &car_owner,
																 const std::string &car_type,
																 const std::string &car_color,
																 const int car_year,
																 const std::string &car_img,
																 const std::string &car_img_type) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"car_id", car_id},
											 {"car_owner", car_owner},
											 {"car_type", car_type},
											 {"car_color", car_color},
											 {"car_year", std::to_string(car_year)},
											 {"car_img", car_img},
											 {"car_img_type", car_img_type}};
	httplib::Result res = client.Post("/add_car", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Add Car] Failed to connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Add Car] Add car to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Add Car] Failed to add car to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Remove a car by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param car_id The ID of the car
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_remove_car(const std::string &ip,
																	const int port,
																	const Account &acc,
																	const std::string &car_id) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {
		{"username", acc.getUsername()}, {"passwd_hash", acc.getPasswdHash()}, {"car_id", car_id}};
	httplib::Result res = client.Post("/remove_car", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Remove Car] Failed to connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Remove Car] Remove car from " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Remove Car] Failed to remove car from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Update a car by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param original_car_id The original ID of the car
 * @param new_car_id The new ID of the car
 * @param new_car_owner The new owner of the car
 * @param new_car_type The new type of the car
 * @param new_car_color The new color of the car
 * @param new_car_year The new year of the car
 * @param new_car_img The new image of the car
 * @param new_car_img_type The new type of the image of the car
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_update_car(
	const std::string &ip,
	const int port,
	const Account &acc,
	const std::string &original_car_id,
	const std::string &new_car_id,
	const std::string &new_car_owner,
	const std::string &new_car_type,
	const std::string &new_car_color,
	const int new_car_year,
	const std::string &new_car_img,
	const std::string &new_car_img_type) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"original_car_id", original_car_id},
											 {"new_car_id", new_car_id},
											 {"new_car_owner", new_car_owner},
											 {"new_car_type", new_car_type},
											 {"new_car_color", new_car_color},
											 {"new_car_year", std::to_string(new_car_year)},
											 {"new_car_img", new_car_img},
											 {"new_car_img_type", new_car_img_type}};
	httplib::Result res = client.Post("/update_car", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Update Car] Failed to connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	MyLogger::log("carinfo-manager-logger",
				  MyLogger::LOG_LEVEL::DEBUG,
				  "[HTTP Update Car] Update car from " + ip + ":" + std::to_string(port) +
					  ". \n- Status Code: " + std::to_string(resp.status) +
					  "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Search an account by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param search_username The username to be searched
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_search_account(
	const std::string &ip, const int port, const Account &acc, const std::string &search_username) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"target_username", search_username}};
	httplib::Result res = client.Post("/get_accountinfo", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Search Account] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200) {
		json resp_json_obj = parse_resp_content(resp);
		if (resp_json_obj.find("error") != resp_json_obj.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::ERROR,
						  "[HTTP Search Account] Failed to search account from " + ip + ":" +
							  std::to_string(port) + ". \n- Status: Invalid Response. \n- Error: " +
							  resp_json_obj["error"].get<std::string>());
			return HttpResult(0, resp_json_obj["error"].get<std::string>());
		}
		else {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::DEBUG,
						  "[HTTP Search Account] Search account from " + ip + ":" +
							  std::to_string(port) + ". \n- Status Code: " +
							  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
			return HttpResult(resp.status, resp.body, resp_json_obj);
		}
	}
	else {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Search Account] Failed to search account from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
		return HttpResult(resp.status, resp.body);
	}
}

/**
 * @brief Get all accounts by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_get_all_accounts(const std::string &ip,
																		  const int port,
																		  const Account &acc) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()}};
	httplib::Result res = client.Post("/get_all_account", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get All Accounts] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200) {
		json resp_json_obj = parse_resp_content(resp);
		if (resp_json_obj.find("error") != resp_json_obj.end()) {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::ERROR,
						  "[HTTP Get All Accounts] Failed to get all accounts from " + ip + ":" +
							  std::to_string(port) + ". \n- Status: Invalid Response. \n- Error: " +
							  resp_json_obj["error"].get<std::string>());
			return HttpResult(0, resp_json_obj["error"].get<std::string>());
		}
		else {
			MyLogger::log("carinfo-manager-logger",
						  MyLogger::LOG_LEVEL::DEBUG,
						  "[HTTP Get All Accounts] Get all accounts from " + ip + ":" +
							  std::to_string(port) + ". \n- Status Code: " +
							  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
			return HttpResult(resp.status, resp.body, resp_json_obj);
		}
	}
	else {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Get All Accounts] Failed to get all accounts from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
		return HttpResult(resp.status, resp.body);
	}
}

/**
 * @brief Add an account by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param new_username The new username of the account
 * @param new_passwd_hash The new password hash of the account
 * @param new_accounttype The new account type of the account
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_add_account(
	const std::string &ip,
	const int port,
	const Account &acc,
	const std::string &new_username,
	const std::string &new_passwd_hash,
	const Account::AccountType new_accounttype) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {
		{"username", acc.getUsername()},
		{"passwd_hash", acc.getPasswdHash()},
		{"target_username", new_username},
		{"target_passwd_hash", new_passwd_hash},
		{"target_account_type", std::to_string((int)new_accounttype)}};
	httplib::Result res = client.Post("/add_account", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Add Account] Failed to connect to " + ip + ":" + std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Add Account] Add account to " + ip + ":" + std::to_string(port) +
						  ". \n- Status Code: " + std::to_string(resp.status) +
						  "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Add Account] Failed to add account to " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Remove an account by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param target_username The username of the account to be removed
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_remove_account(
	const std::string &ip, const int port, const Account &acc, const std::string &target_username) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"passwd_hash", acc.getPasswdHash()},
											 {"target_username", target_username}};
	httplib::Result res = client.Post("/remove_account", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Remove Account] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Remove Account] Remove account from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Remove Account] Failed to remove account from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Update an account by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param target_username The username of the account to be updated
 * @param new_username The new username of the account
 * @param new_passwd_hash The new password hash of the account
 * @param new_accounttype The new account type of the account
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_update_account(
	const std::string &ip,
	const int port,
	const Account &acc,
	const std::string &target_username,
	const std::string &new_username,
	const std::string &new_passwd_hash,
	const Account::AccountType new_accounttype) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {
		{"username", acc.getUsername()},
		{"passwd_hash", acc.getPasswdHash()},
		{"target_username", target_username},
		{"new_username", new_username},
		{"new_passw_hash", new_passwd_hash},
		{"new_account_type", std::to_string((int)new_accounttype)}};
	httplib::Result res = client.Post("/update_account", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Update Account] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Update Account] Update account from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Update Account] Failed to update account from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
}

/**
 * @brief Change the password of an account by sending a request to the server
 * 
 * @param ip The IP address of the server
 * @param port The port of the server
 * @param acc The account to be verified
 * @param new_passwd_hash The new password hash of the account
 * 
 * @return HttpResult The result of the HTTP request
 */
ClientHttpHandler::HttpResult ClientHttpHandler::handler_change_password(
	const std::string &ip, const int port, const Account &acc, const std::string &new_passwd_hash) {
	httplib::Client client(ip, port);
	client.set_read_timeout(5);

	httplib::MultipartFormDataItems items = {{"username", acc.getUsername()},
											 {"old_passwd_hash", acc.getPasswdHash()},
											 {"new_passwd_hash", new_passwd_hash}};
	httplib::Result res = client.Post("/change_password", items);
	if (!res) {
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Change Password] Failed to connect to " + ip + ":" +
						  std::to_string(port) +
						  ". \n- Error: " + std::to_string((int)res.error()));
		return HttpResult(0, std::to_string((int)(res.error())));
	}
	httplib::Response resp = res.value();
	if (resp.status == 200)
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::DEBUG,
					  "[HTTP Change Password] Change password from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	else
		MyLogger::log("carinfo-manager-logger",
					  MyLogger::LOG_LEVEL::ERROR,
					  "[HTTP Change Password] Failed to change password from " + ip + ":" +
						  std::to_string(port) + ". \n- Status Code: " +
						  std::to_string(resp.status) + "\n- Response Body: " + resp.body);
	return HttpResult(resp.status, resp.body);
	return HttpResult(resp.status, resp.body);
}

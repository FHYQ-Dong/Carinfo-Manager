/**
 * @file src/httphandler.cpp
 * @brief Implementation of class HttpHandler for the server of carinfo-manager project
 * 
 * @details
 * This file contains the implementation of the HttpHandler class, which is responsible for handling various HTTP requests
 * in the carinfo-manager server. It includes functions for handling test connection requests, login requests, changing passwords,
 * retrieving car information, retrieving car images, and adding cars to the system.
 * The HttpHandler class interacts with the AccountPool and CarPool classes to perform the necessary operations for each request.
 * It also uses the httplib library for handling HTTP requests and responses.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/httphandler.hpp"
#include "carinfo-manager/base64.hpp"
#include "json/json.hpp"
#include <sstream>

using json = nlohmann::json;

/**
 * @brief Construct a new HttpHandler object
 * 
 * @param accountpool The AccountPool object for managing user accounts
 * @param carpool The CarPool object for managing car information
 * @param imgDir The directory path for storing car images
 */
HttpHandler::HttpHandler(AccountPool& accountpool, CarPool& carpool, std::string imgDir): \
    accountpool(accountpool), carpool(carpool), imgDir(imgDir) {
}

/**
 * @brief Parse the POST body of an HTTP request
 * 
 * This function parses the POST body of an HTTP request and returns the content as a JSON object.
 * 
 * @param req The HTTP request object
 * @return json The parsed JSON object containing the POST body content
 */
json HttpHandler::parse_post_body(const httplib::Request &req) const {
    json j;
    for (const auto& file : req.files)
        j[file.first] = file.second.content;
    return j;
}

/**
 * Handles the test connection request.
 * 
 * This function is responsible for handling the test connection request and
 * returning a response with a status code of 200 and a content of "Connection is OK".
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_test_connection(const httplib::Request &req, httplib::Response &res) const {
    res.set_content("Connection is OK", "text/plain");
    res.status = 200;
}

/**
 * Handles the HTTP login request from the client.
 * 
 * @param req The HTTP request object containing the login parameters.
 * @param res The HTTP response object to send back to the client.
 */
void HttpHandler::handler_login(const httplib::Request &req, httplib::Response &res) const {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd = std::string(params["passwd_hash"]);

    auto result = accountpool.verifyAccount(username, passwd);
    if (result == AccountPool::AccountVerifyResult::SUCCESS) {
        res.set_content("Login Success", "text/plain");
        res.status = 200;   
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Account Not Found or Wrong Password", "text/plain");
        res.status = 401;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the HTTP request for changing password.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_change_password(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("old_passwd_hash") == params.end() || params.find("new_passwd_hash") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string old_passwd_hash = std::string(params["old_passwd_hash"]);
    std::string new_passwd_hash = std::string(params["new_passwd_hash"]);

    try {
        auto result = accountpool.verifyAccount(username, old_passwd_hash);
        if (result == AccountPool::AccountVerifyResult::SUCCESS) {
            Account old_acc = accountpool.getAccount(username);
            Account new_acc = Account(username, new_passwd_hash, old_acc.getAccountType());
            int status_code = accountpool.updateAccount(old_acc, new_acc);
            if(status_code == 0) {
                res.set_content("Password Changed", "text/plain");
                res.status = 200;
            }
            else {
                std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
                res.set_content(msg, "text/plain");
                res.status = 500;
            }
        }
        else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
                result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
            res.set_content("Forbidden", "text/plain");
            res.status = 403;
        }
        else {
            res.set_content("Internal Server Error", "text/plain");
            res.status = 500;
        }
    }
    catch (...) {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the HTTP request for retrieving car information.
 *
 * @param req The HTTP request object containing the request parameters.
 * @param res The HTTP response object to be sent back to the client.
 */
void HttpHandler::handler_get_carinfo(const httplib::Request &req, httplib::Response &res) const {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("car_id") == params.end() || params.find("car_owner") == params.end() || \
        params.find("car_color") == params.end() || params.find("car_type") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string car_id = std::string(params["car_id"]);
    std::string car_owner = std::string(params["car_owner"]);
    std::string car_color = std::string(params["car_color"]);
    std::string car_type = std::string(params["car_type"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS) {
        CarPool cars = carpool.getCar(car_id, car_color, car_owner, car_type);
        std::ostringstream os;
        cars.save(os);
        res.set_content(os.str(), "application/json");
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the HTTP request for retrieving a car image.
 *
 * This function is responsible for handling the POST request to retrieve a car image. It verifies the user's account
 * credentials, checks if the specified car image file exists, and returns the image content in base64 format if the
 * file is found. If the file is not found or if there is an error during the account verification process, appropriate
 * HTTP response status codes and content are set.ww
 *
 * @param req The HTTP request object containing the request parameters.
 * @param res The HTTP response object to be populated with the response data.
 */
void HttpHandler::handler_get_carimg(const httplib::Request &req, httplib::Response &res) const {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("car_img_path") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string car_img_path = std::string(params["car_img_path"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS) {
        std::ifstream is(car_img_path, std::ios::binary);
        if (is.is_open()) {
            std::string img((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
            if (car_img_path.substr(car_img_path.size() - 4) == ".png") 
                res.set_content(img.c_str(), img.size(), "image/png");
            else if (car_img_path.substr(car_img_path.size() - 4) == ".jpg") 
                res.set_content(img.c_str(), img.size(), "image/jpeg");
            else
                res.set_content(img.c_str(), img.size(), "image/jpeg");
            res.status = 200;
            is.close();
        }
        else {
            res.set_content("File Not Found", "text/plain");
            res.status = 404;
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the HTTP request for adding a car.
 *
 * This function is responsible for processing the HTTP request to add a car to the system.
 * It retrieves the necessary parameters from the request, verifies the user account,
 * saves the car image to the appropriate location, creates a new Car object, and adds it to the car pool.
 * The function sets the appropriate response content and status code based on the result of the operation.
 *
 * @param req The HTTP request object containing the necessary parameters.
 * @param res The HTTP response object to be modified based on the result of the operation.
 */
void HttpHandler::handler_add_car(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("car_id") == params.end() || params.find("car_type") == params.end() || \
        params.find("car_owner") == params.end() || params.find("car_color") == params.end() || \
        params.find("car_year") == params.end() || params.find("car_img") == params.end() || \
        params.find("car_img_type") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string car_id = std::string(params["car_id"]);
    std::string car_type = std::string(params["car_type"]);
    std::string car_owner = std::string(params["car_owner"]);
    std::string car_color = std::string(params["car_color"]);
    int car_year = std::stoi(std::string(params["car_year"]));
    std::string car_img = std::string(params["car_img"]);
    std::string car_img_type = std::string(params["car_img_type"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        std::string car_id_base64 = Base64::encode(reinterpret_cast<const unsigned char*>(car_id.c_str()), car_id.size());
        for (size_t i = 0; i < car_id_base64.size(); i++) {
            if (car_id_base64[i] == '/') car_id_base64[i] = '_';
            if (car_id_base64[i] == '+') car_id_base64[i] = '-';
        }
        std::string car_img_path = imgDir + car_id_base64 + car_img_type;
        std::ofstream os(car_img_path, std::ios::binary);
        os.write(car_img.c_str(), car_img.size());
        os.close();
        Car new_car = Car(car_id, car_type, car_owner, car_color, car_year, car_img_path);
        int status_code = carpool.addCar(new_car);
        if (status_code == 0) {
            res.set_content("Car Added", "text/plain");
            res.status = 200;
        }
        else {
            std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
            res.set_content(msg, "text/plain");
            res.status = 500;
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the removal of a car based on the provided parameters.
 * 
 * @param req The HTTP request object containing the parameters.
 * @param res The HTTP response object to be modified.
 */
void HttpHandler::handler_remove_car(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("car_id") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string car_id = std::string(params["car_id"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        CarPool cars = carpool.getCarbyId(car_id);
        if (cars.size() == 0) {
            res.set_content("Car Not Found", "text/plain");
            res.status = 404;
        }
        else {
            int status_code = carpool.removeCar(car_id);
            if (status_code == 0) {
                res.set_content("Car Removed", "text/plain");
                res.status = 200;
            }
            else {
                std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
                res.set_content(msg, "text/plain");
                res.status = 500;
            }
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the update car request.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_update_car(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("original_car_id") == params.end() || params.find("new_car_id") == params.end() || \
        params.find("new_car_type") == params.end() || params.find("new_car_owner") == params.end() || \
        params.find("new_car_color") == params.end() || params.find("new_car_year") == params.end() || \
        params.find("new_car_img") == params.end() || params.find("new_car_img_type") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string original_car_id = std::string(params["original_car_id"]);
    std::string new_car_id = std::string(params["new_car_id"]);
    std::string new_car_type = std::string(params["new_car_type"]);
    std::string new_car_owner = std::string(params["new_car_owner"]);
    std::string new_car_color = std::string(params["new_car_color"]);
    int new_car_year = std::stoi(std::string(params["new_car_year"]));
    std::string new_car_img = std::string(params["new_car_img"]);
    std::string new_car_img_type = std::string(params["new_car_img_type"]);
    
    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        CarPool original_cars = carpool.getCarbyId(original_car_id);
        if (original_cars.size() == 0) {
            res.set_content("Car Not Found", "text/plain");
            res.status = 404;
        }
        else {
            std::string new_car_id_base64 = Base64::encode(reinterpret_cast<const unsigned char*>(new_car_id.c_str()), new_car_id.size());
            for (size_t i = 0; i < new_car_id_base64.size(); i++) {
                if (new_car_id_base64[i] == '/') new_car_id_base64[i] = '_';
                if (new_car_id_base64[i] == '+') new_car_id_base64[i] = '-';
            }
            std::string new_car_img_path = imgDir + new_car_id_base64 + new_car_img_type;
            std::ofstream os(new_car_img_path, std::ios::binary);
            os.write(new_car_img.c_str(), new_car_img.size());
            os.close();
            Car new_car = Car(new_car_id, new_car_type, new_car_owner, new_car_color, new_car_year, new_car_img_path);
            int status_code = carpool.updateCar(original_car_id, new_car);
            if (status_code == 0) {
                res.set_content("Car Updated", "text/plain");
                res.status = 200;
            }
            else {
                std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
                res.set_content(msg, "text/plain");
                res.status = 500;
            }
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the request for retrieving account information.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_get_accountinfo(const httplib::Request &req, httplib::Response &res) const {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("target_username") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string target_username = std::string(params["target_username"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        Account target_acc = accountpool.getAccount(target_username);
        if (target_acc == Account::NULL_ACCOUNT) {
            res.set_content("Account Not Found", "text/plain");
            res.status = 404;
        }
        else {
            nlohmann::json j;
            j["username"] = target_acc.getUsername();
            j["passwd_hash"] = target_acc.getPasswdHash();
            j["account_type"] = target_acc.getAccountType();
            res.set_content(j.dump(4), "application/json");
            res.status = 200;
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the request for adding an account.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_add_account(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("target_username") == params.end() || params.find("target_passwd_hash") == params.end() || \
        params.find("target_account_type") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string target_username = std::string(params["target_username"]);
    std::string target_passwd_hash = std::string(params["target_passwd_hash"]);
    int target_account_type = std::stoi(std::string(params["target_account_type"]));

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        Account new_acc = Account(target_username, target_passwd_hash, static_cast<Account::AccountType>(target_account_type));
        int status_code = accountpool.addAccount(new_acc);
        if (status_code == 0) {
            res.set_content("Account Added", "text/plain");
            res.status = 200;
        }
        else {
            std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
            res.set_content(msg, "text/plain");
            res.status = 500;
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the request for removing an account.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_remove_account(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("target_username") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string target_username = std::string(params["target_username"]);

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        Account target_acc = accountpool.getAccount(target_username);
        if (target_acc == Account::NULL_ACCOUNT) {
            res.set_content("Account Not Found", "text/plain");
            res.status = 404;
        }
        else {
            int status_code = accountpool.removeAccount(target_username);
            if (status_code == 0) {
                res.set_content("Account Removed", "text/plain");
                res.status = 200;
            }
            else {
                std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
                res.set_content(msg, "text/plain");
                res.status = 500;
            }
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

/**
 * Handles the request for updating an account.
 * 
 * @param req The HTTP request object.
 * @param res The HTTP response object.
 */
void HttpHandler::handler_update_account(const httplib::Request &req, httplib::Response &res) {
    json params = parse_post_body(req);
    if (params.find("username") == params.end() || params.find("passwd_hash") == params.end() || \
        params.find("target_username") == params.end() || params.find("new_username") == params.end() || \
        params.find("new_passwd_hash") == params.end() || params.find("new_account_type") == params.end()) {
        res.set_content("Bad Request", "text/plain");
        res.status = 400;
        return;
    }
    std::string username = std::string(params["username"]);
    std::string passwd_hash = std::string(params["passwd_hash"]);
    std::string target_username = std::string(params["target_username"]);
    std::string new_username = std::string(params["new_username"]);
    std::string new_passwd_hash = std::string(params["new_passwd_hash"]);
    int new_account_type = std::stoi(std::string(params["new_account_type"]));

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        Account target_acc = accountpool.getAccount(target_username);
        if (target_acc == Account::NULL_ACCOUNT) {
            res.set_content("Account Not Found", "text/plain");
            res.status = 404;
        }
        else {
            Account new_acc = Account(new_username, new_passwd_hash, static_cast<Account::AccountType>(new_account_type));
            int status_code = accountpool.updateAccount(target_acc, new_acc);
            if (status_code == 0) {
                res.set_content("Account Updated", "text/plain");
                res.status = 200;
            }
            else {
                std::string msg = "Internal Server Error, status code: " + std::to_string(status_code);
                res.set_content(msg, "text/plain");
                res.status = 500;
            }
        }
    }
    else if (result == AccountPool::AccountVerifyResult::ACCOUNT_NOT_FOUND || \
            result == AccountPool::AccountVerifyResult::WRONG_PASSWORD) {
        res.set_content("Forbidden", "text/plain");
        res.status = 403;
    }
    else {
        res.set_content("Internal Server Error", "text/plain");
        res.status = 500;
    }
}

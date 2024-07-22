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
#include <sys/stat.h>

HttpHandler::HttpHandler(AccountPool& accountpool, CarPool& carpool, std::string dataDir): \
    accountpool(accountpool), carpool(carpool), dataDir(dataDir) {
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
void HttpHandler::handler_test_connection(const httplib::Request &req, httplib::Response &res) {
    res.set_content("Connection is OK", "text/plain");
    res.status = 200;
}

/**
 * Handles the HTTP login request from the client.
 * 
 * @param req The HTTP request object containing the login parameters.
 * @param res The HTTP response object to send back to the client.
 */
void HttpHandler::handler_login(const httplib::Request &req, httplib::Response &res) {
    std::string username = req.get_param_value("username");
    std::string passwd = req.get_param_value("passwd_hash");

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
    std::string username = req.get_param_value("username");
    std::string old_passwd_hash = req.get_param_value("old_passwd_hash");
    std::string new_passwd_hash = req.get_param_value("new_passwd_hash");

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
void HttpHandler::handler_get_carinfo(const httplib::Request &req, httplib::Response &res) {
    std::string username = req.get_param_value("username");
    std::string passwd_hash = req.get_param_value("passwd_hash");
    std::string car_id = req.get_param_value("car_id");
    std::string car_color_str = req.get_param_value("car_color");
    Color car_color = Color(car_color_str);
    std::string car_type = req.get_param_value("car_type");

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS) {
        CarPool cars = carpool.getCar(car_id, car_color, car_type);
        size_t ostreamSize = cars.ostreamSize();
        std::ostringstream os(std::string(ostreamSize, '\0'), std::ios::binary);
        cars.save(os);
        std::string base64res = Base64::encode((u_char*)os.str().c_str(), ostreamSize);
        res.set_content(base64res, "text/plain");
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
 * This function is responsible for handling the GET request to retrieve a car image. It verifies the user's account
 * credentials, checks if the specified car image file exists, and returns the image content in base64 format if the
 * file is found. If the file is not found or if there is an error during the account verification process, appropriate
 * HTTP response status codes and content are set.
 *
 * @param req The HTTP request object containing the request parameters.
 * @param res The HTTP response object to be populated with the response data.
 */
void HttpHandler::handler_get_carimg(const httplib::Request &req, httplib::Response &res) {
    std::string username = req.get_param_value("username");
    std::string passwd_hash = req.get_param_value("passwd_hash");
    std::string car_img_path = req.get_param_value("car_img_path");

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS) {
        struct stat buffer;
        if (stat(car_img_path.c_str(), &buffer) == 0) {
            std::ifstream is(car_img_path, std::ios::binary);
            std::string img((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
            std::string base64res = Base64::encode((u_char*)img.c_str(), img.size());
            res.set_content(base64res, "text/plain");
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
    std::string username = req.get_param_value("username");
    std::string passwd_hash = req.get_param_value("passwd_hash");
    std::string car_id = req.get_param_value("car_id");
    std::string car_type = req.get_param_value("car_type");
    std::string car_color_str = req.get_param_value("car_color");
    Color car_color = Color(car_color_str);
    int car_year = std::stoi(req.get_param_value("car_year"));
    std::string car_img_base64 = req.get_param_value("car_img_base64");
    std::string car_img_type = req.get_param_value("car_img_type"); // .png, .jpg, etc.

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        std::string car_img_path = dataDir + "car_img/" + car_id + car_img_type;
        std::ofstream os(car_img_path, std::ios::binary);
        size_t decodeSize = 0;
        std::string car_img = Base64::decode(car_img_base64.c_str(), car_img_base64.size(), decodeSize);
        os.write(car_img.c_str(), decodeSize);
        os.close();
        Car new_car = Car(car_id, car_type, car_color, car_year, car_img_path);
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
    std::string username = req.get_param_value("username");
    std::string passwd_hash = req.get_param_value("passwd_hash");
    std::string car_id = req.get_param_value("car_id");

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
    std::string username = req.get_param_value("username");
    std::string passwd_hash = req.get_param_value("passwd_hash");
    std::string original_car_id = req.get_param_value("original_car_id");
    std::string new_car_id = req.get_param_value("new_car_id");
    std::string car_type = req.get_param_value("car_type");
    std::string car_color_str = req.get_param_value("car_color");
    Color car_color = Color(car_color_str);
    int car_year = std::stoi(req.get_param_value("car_year"));
    std::string car_img_base64 = req.get_param_value("car_img_base64");
    std::string car_img_type = req.get_param_value("car_img_type"); // .png, .jpg, etc.

    auto result = accountpool.verifyAccount(username, passwd_hash);
    if (result == AccountPool::AccountVerifyResult::SUCCESS && accountpool.getAccountType(username) == Account::AccountType::ADMIN) {
        CarPool original_cars = carpool.getCarbyId(original_car_id);
        if (original_cars.size() == 0) {
            res.set_content("Car Not Found", "text/plain");
            res.status = 404;
        }
        else {
            std::string car_img_path = dataDir + "car_img/" + new_car_id + car_img_type;
            std::ofstream os(car_img_path, std::ios::binary);
            size_t decodeSize = 0;
            std::string car_img = Base64::decode(car_img_base64.c_str(), car_img_base64.size(), decodeSize);
            os.write(car_img.c_str(), decodeSize);
            os.close();
            Car new_car = Car(new_car_id, car_type, car_color, car_year, car_img_path);
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

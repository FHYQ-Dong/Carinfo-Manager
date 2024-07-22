/**
 * @file httphandler.hpp
 * @brief Declaration of class HttpHandler
 * 
 * @details
 * This file contains the declaration of the HttpHandler class, which handles HTTP requests for car information management.
 * The HttpHandler class provides methods for handling various types of requests, such as testing the connection, login or password change, car management, and account management.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/accountpool.hpp"
#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/hash.hpp"
#include "carinfo-manager/base64.hpp"
#include "cpp-httplib/httplib.h"


class HttpHandler {
    private:
        AccountPool& accountpool;
        CarPool& carpool;
        std::string dataDir; // end with '/'

    public:
        HttpHandler(AccountPool& accountpool, CarPool& carpool, std::string dataDir);
        // test connection
        void handler_test_connection(const httplib::Request &req, httplib::Response &res);
        // login or change password
        void handler_login(const httplib::Request &req, httplib::Response &res);
        void handler_change_password(const httplib::Request &req, httplib::Response &res);
        // car management
        void handler_get_carinfo(const httplib::Request &req, httplib::Response &res);
        void handler_get_carimg(const httplib::Request &req, httplib::Response &res);
        void handler_add_car(const httplib::Request &req, httplib::Response &res);
        void handler_remove_car(const httplib::Request &req, httplib::Response &res);
        void handler_update_car(const httplib::Request &req, httplib::Response &res);
        // account management
        void handler_get_accountinfo(const httplib::Request &req, httplib::Response &res);
        void handler_add_account(const httplib::Request &req, httplib::Response &res);
        void handler_remove_account(const httplib::Request &req, httplib::Response &res);
        void handler_update_account(const httplib::Request &req, httplib::Response &res);
};

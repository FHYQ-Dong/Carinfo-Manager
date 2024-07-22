/**
 * @file src/server.cpp
 * @brief Implementation of a server for the carinfo-manager project using httplib library
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/carpool.hpp"
#include "carinfo-manager/hash.hpp"
#include <fstream>
#include "carinfo-manager/accountpool.hpp"
#include "cpp-httplib/httplib.h"
#include <iostream>

using namespace std;


int main() {
    httplib::Server svr;

    svr.Get("/test", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_content("asdfghj\0 123456", "application/octet-stream");
        res.status = 200;
    });

    svr.listen("localhost", 8080);

    return 0;
}

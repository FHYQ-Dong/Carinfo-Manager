/**
 * @file src/client.cpp
 * @brief Implementation of a client for the server of carinfo-manager project
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include <iostream>
#include "cpp-httplib/httplib.h"
#include <string>
#include "carinfo-manager/hash.hpp"
using namespace std;

int main() {
    cout << "Hi, this is a client." << endl;

    httplib::Client cli("localhost", 8080);
    auto res = cli.Get("/test_connection");
    if (res && res->status == 200) {
        cout << res->body << endl;
    }
    else {
        cout << "Connection failed" << endl;
    }

    httplib::Params params;
    params.emplace("username", "admin");
    params.emplace("passwd_hash", Hash("admin1", "donghy23").getHash());
    res = cli.Post("/login", params);
    cout << res->status << endl;
    cout << res->body << endl;
    return 0;
}

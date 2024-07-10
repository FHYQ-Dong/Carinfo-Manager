#include "gtest/gtest.h"
#include "carinfo-manager/base64.hpp"
#include <iostream>

TEST(Test_Base64, TEST_Base64_encode) {
    char* tmp = "ABCXABC";
    unsigned char* src = new unsigned char[8];
    for (size_t i = 0; i < 8; i++) src[i] = (unsigned char)(tmp[i]);
    src[3] = '\0'; src[7] = '\0';
    std::string res = Base64::encode(src, 7);
    std::string ans = "QUJDAEFCQw==";
    for (size_t i = 0; i < ans.length(); i++) 
        EXPECT_TRUE(res[i] == ans[i]);
    EXPECT_EQ(ans.length(), res.length());
}

TEST(Test_Base64, TEST_Base64_decode) {
    char* src = "QUJDAEFCQw==";
    int a = 0;
    size_t res_len = 0;
    std::string res = Base64::decode(src, 12, res_len);
    std::string ans = "ABC";
    ans += '\0';
    ans += "ABC";
    for (size_t i = 0; i < ans.length(); i++) 
        EXPECT_TRUE(ans[i] == res[i]);
    EXPECT_EQ(ans.length(), res_len);
}
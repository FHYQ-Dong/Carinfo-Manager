#include "carinfo-manager/hash.hpp"
#include "gtest/gtest.h"

TEST(Test_Hash, test_Hash_original) {
    Hash hash("test1", "");
    EXPECT_TRUE(hash.getHash() == "1b4f0e9851971998e732078544c96b36c3d01cedf7caa332359d6f1d83567014");
    hash.setData("test2");
    EXPECT_TRUE(hash.getHash() == "60303ae22b998861bce3b28f33eec1be758a213c86c93c076dbe9f558c11c752");
    hash.setData("test3");
    EXPECT_TRUE(hash.getHash() == "fd61a03af4f77d870fc21e05e7e80678095c92d808cfb3b5c279ee04c74aca13");
}

TEST(Test_Hash, test_Hash_salt) {
    Hash hash("test1", "salt1");
    EXPECT_TRUE(hash.getHash() == "a1cbb225c3c0cd1cdf283c22ed489639acfeff291f73072a1cdbab11697edcd8");
    hash.setData("test2"); hash.setSalt("salt2");
    EXPECT_TRUE(hash.getHash() == "cdec2598121529d63c0aafced05a7a86e099f794b8889c9528df0cd171e2fbb5");
    hash.setData("test3"); hash.setSalt("salt3");
    EXPECT_TRUE(hash.getHash() == "c7d9e34aba1b7aadd65410ac6f5e4048547476be69953ccf31060eb88bea1b3f");
}

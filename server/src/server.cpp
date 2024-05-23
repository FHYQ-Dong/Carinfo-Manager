#include "CarPool.hpp"
#include "Hash.hpp"
#include <iostream>

int main() {
    Hash hashObj("Hello, World!", "salt");
    std::cout << "Hash of 'Hello, World!': " << hashObj.getHash() << std::endl;
    hashObj.setData("Hello, World!");
    std::cout << "Hash of 'Hello, World!': " << hashObj.getHash() << std::endl;
    hashObj.setSalt("salt");
    std::cout << "Hash of 'Hello, World!': " << hashObj.getHash() << std::endl;
    std::cout << "length of hash: " << hashObj.getHash().length() << std::endl;
    return 0;
}
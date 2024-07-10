#pragma once
#include <string>

class Base64 {
    private:
        static const char encodeTable[];
        static const char decodeTable[];
    public:
        static std::string encode(const unsigned char* data, size_t data_bytes_length);
        static std::string decode(const char* data, int data_bytes_length, size_t& output_bytes_length);
};
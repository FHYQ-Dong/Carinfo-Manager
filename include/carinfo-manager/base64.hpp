/**
 * @file include/carinfo-manager/base64.hpp
 * @brief Declaration of class Base64
 *
 * @details
 * This file contains the declaration of the Base64 class. The Base64 class provides static methods to encode and decode data using the Base64 encoding scheme.
 * The Base64 encoding scheme is commonly used to encode binary data as ASCII text, which can be easily transmitted over text-based protocols such as email or HTTP.
 * The class provides two static methods:
 *     - `encode`: Encodes a given byte array into a Base64-encoded string.
 *     - `decode`: Decodes a Base64-encoded string into its original byte array.
 * The class also defines two private static member variables:
 *     - `encodeTable`: A lookup table used for encoding.
 *     - `decodeTable`: A lookup table used for decoding.
 * This class is designed to be used as a utility class and does not require any instantiation.
 *
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0 
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <string>

class Base64 {
    private:
        static const char encodeTable[];
        static const char decodeTable[];
    public:
        static std::string encode(const unsigned char* data, size_t data_bytes_length);
        static std::string decode(const char* data, size_t data_bytes_length, size_t& output_bytes_length);
};

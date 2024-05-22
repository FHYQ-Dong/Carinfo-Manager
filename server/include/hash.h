#pragma once
#include <string>
#include <cstdint>

class Hash {
    private:
        std::string data, salt, hash;
        void sha256();
        
    private:
        const static uint32_t K[64];
        const static int a, b, c, d, e, f, g, h;
        uint32_t right_rotate(uint32_t x, uint32_t n);
        uint32_t little_sigma_0(uint32_t x);
        uint32_t little_sigma_1(uint32_t x);
        uint32_t big_sigma_0(uint32_t x);
        uint32_t big_sigma_1(uint32_t x);
        void message_schedule(uint32_t(&W)[64], uint8_t(&block)[64]);
        uint32_t choice(uint32_t x, uint32_t y, uint32_t z);
        uint32_t majority(uint32_t x, uint32_t y, uint32_t z);
        void round(uint32_t(&H)[8], uint32_t round_constant, uint32_t schedule_word);
        void compress_block(uint32_t(&H)[8], uint8_t(&block)[64]);
    
    public:
        Hash();
        Hash(std::string data, std::string salt);
        Hash(const Hash& hashObj);
        ~Hash();
        void setData(std::string data);
        void setSalt(std::string salt);
        std::string getHash();
};
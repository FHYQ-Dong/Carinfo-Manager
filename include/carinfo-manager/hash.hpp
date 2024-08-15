/**
 * @file include/carinfo-manager/hash.hpp
 * @brief Declaration of class Hash, which is used to hash a string with salt using SHA-256 algorithm
 * 
 * @details
 * This file contains the declaration of the Hash class. The Hash class provides functionality to hash a string with a salt using the SHA-256 algorithm.
 * The class includes private member variables for storing the data, salt, and hash. It also includes private member functions for performing various operations related to the SHA-256 algorithm.
 * Public member functions are provided to set the data and salt, retrieve the hash, and perform other operations related to hashing.
 * The Hash class is designed to be used in the Carinfo-Manager project.
 * 
 * @author donghy23@mails.tsinghua.edu.cn & kibonga@github.com
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>
#include <cstring>
#include <string>

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
	void message_schedule(uint32_t (&W)[64], uint8_t (&block)[64]);
	uint32_t choice(uint32_t x, uint32_t y, uint32_t z);
	uint32_t majority(uint32_t x, uint32_t y, uint32_t z);
	void round(uint32_t (&H)[8], uint32_t round_constant, uint32_t schedule_word);
	void compress_block(uint32_t (&H)[8], uint8_t (&block)[64]);

  public:
	Hash();
	Hash(std::string data, std::string salt);
	Hash(const Hash &hashObj);
	~Hash();
	void setData(std::string data);
	void setSalt(std::string salt);
	std::string getHash();
};

#pragma once

#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/evperr.h>

class Encryptor {
public:
	/*
	 * @brief: ncrypt a string
	 * @param input: Reference to a string that is to be compressed
	 * @returns Compressed string
	 */
	virtual void encrypt() = 0;
	virtual void decrypt() = 0;
};

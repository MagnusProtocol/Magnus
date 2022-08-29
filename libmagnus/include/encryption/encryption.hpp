#pragma once

#include <iostream>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string_view>

// Crypto++
#include <eccrypto.h>
#include <ecp.h>
#include <magnus.hpp>
#include <osrng.h>
#include <xed25519.h>
using namespace CryptoPP;

namespace Magnus::LibMagnus
{
struct Keys
{
	// Diffie-hellman key, initalized by class constructor
	ECDH<ECP>::Domain dh_key;
	// Private/public keys
	SecByteBlock private_key, public_key;
	// Initialize the values
	Keys()
	{
		dh_key = ECDH<ECP>::Domain(ASN1::brainpoolP256r1());
		private_key.New(dh_key.PrivateKeyLength());
		public_key.New(dh_key.PublicKeyLength());
	}
};

class Encryption
{
	private:
	// Private/Public keys
	Keys mKeys;
	SecByteBlock mSharedKey;
	OID mCurve;
	AutoSeededRandomPool mRng;

	// Called in every constructor, sets common values.
	void setup();

	public:
	/*
	 * @brief: return the keys stored in the private class variable.
	 * @returns: returns a fully populated object of type, "Keys"
	 */
	Keys &get_keys();

	/*
	 * @brief: Decode the shared key obtained by using the generate_shared_key() function
	 * @param keys: The shared key.
	 * @returns: The shared key, as a CryptoPP::Integer.
	 */
	Integer decode_shared_key(SecByteBlock &shared_key);

	// Generate a shared key
	SecByteBlock generate_shared_key(Keys &pKeys);

	// AES-256
	void encrypt_data_aes_256(SecByteBlock &key, std::string &data, byte *out_data,
							  const CryptoPP::byte *iv);
	void decrypt_data_aes_256(SecByteBlock &key, std::string &data, byte *out_data,
							  const CryptoPP::byte *iv);
	void decrypt_data_aes_256(SecByteBlock &key, byte *data, size_t length, byte *out_data,
							  const CryptoPP::byte *iv);

	Encryption();
	~Encryption();
};

void string_to_byte(std::string &data, byte *data_bytes);
}; // namespace Magnus::LibMagnus

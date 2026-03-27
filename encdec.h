/**
* @brief	This file hosts an abstraction of the Crypto++ library, by providing a higher-level interface for encryption and decryption of data.
* @author	jjonn0
* 
* Every time encrypt() is ran, a 32 char string is appended to the front. This is the iv, which is generated every time encrypt() is ran.
* 
* Issues:
* - During generateBytes(), bits are read backwards. Technically, this is a bug, but since it doesn't really change anything this is ignored.
*/
#pragma once
#include <iostream>
#include <array>
#include <bitset>
#include <format>

#include "cryptlib.h"
#include "rijndael.h"
#include "aes.h"
#include "osrng.h"
#include "hex.h"
#include "sha.h"
#include "modes.h"
#include "eax.h"
#include "files.h"

constexpr size_t BYTE_SIZE{8};
constexpr size_t KEY_LENGTH{CryptoPP::AES::DEFAULT_KEYLENGTH};

// Set to true to enable debug mode. Prints results into terminal.
constexpr bool DEBUG_MODE{false};

class EncDec
{
	private:
	CryptoPP::SecByteBlock m_iv;

	public:
	explicit EncDec()
	{
		m_iv = generateIV();
	}

	CryptoPP::SecByteBlock generateIV()
	{
		CryptoPP::AutoSeededRandomPool asrl;
		CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
		asrl.GenerateBlock(iv, iv.size());
		return iv;
	}

	std::bitset<BYTE_SIZE * KEY_LENGTH> generateBytes(const std::string& data)
	{
		const size_t total_bits{ BYTE_SIZE * KEY_LENGTH };
		std::string remaining_string{ data };
		std::bitset<total_bits> bits;

		for (size_t remaining_string_index{ 0 }; remaining_string_index < remaining_string.size(); remaining_string_index++)
		{
			// Grab the next char
			char remaining_string_char{ remaining_string[remaining_string_index] };

			// Move to the start of the byte
			size_t bit_index{ remaining_string_index * BYTE_SIZE };
			// If the index exceeds the length of the key, move back to the start
			bit_index %= total_bits;
			// Create a bit representation of the char
			std::bitset<BYTE_SIZE> char_bits{ size_t(remaining_string_char) };

			if (DEBUG_MODE) std::cout << std::format("Looking at char {} : [{}]\n", remaining_string_char, remaining_string_index);


			// Keep looping until we have covered a full byte of data
			do
			{
				size_t char_bit_index{ bit_index - ((remaining_string_index * BYTE_SIZE) % total_bits) };

				if (DEBUG_MODE) std::cout << std::format("Accessing key index: {}, value: {}. Accessing char index: {}, value: {}. XOR result: ", bit_index, int(bits[bit_index]), char_bit_index, int(char_bits[char_bit_index]));

				bits.set(bit_index, bits[bit_index] ^ char_bits[char_bit_index]);

				if (DEBUG_MODE) std::cout << bits[bit_index] << std::endl;

				bit_index++;
			} while (bit_index % BYTE_SIZE != 0);
		}

		if (DEBUG_MODE) std::cout << std::format("Resulting bits: {}", bits.to_string()) << std::endl;

		return bits;
	}

	std::string bitsToString(const std::bitset<BYTE_SIZE* KEY_LENGTH> bits)
	{
		std::string string_out{ "" };

		for (size_t byte_index{ 0 }; byte_index < KEY_LENGTH; byte_index++)
		{
			size_t bit_index{ 0 };
			int char_value{ 0 };
			do
			{
				char_value += bits[byte_index * BYTE_SIZE + bit_index] * pow(2, bit_index);
				bit_index++;
			} while (bit_index % BYTE_SIZE != 0);
			string_out += char(char_value);
		}

		return string_out;
	}

	//CryptoPP::SecByteBlock
	void stringToKey(std::string password)
	{
		// Truncate password
		std::bitset<BYTE_SIZE* KEY_LENGTH> password_bits{ generateBytes(password) };
		std::string key_string{ bitsToString(password_bits) };
	}

	std::string encrypt(std::string password, std::string plaintext)
	{

		std::bitset<BYTE_SIZE* KEY_LENGTH> password_bytes{ generateBytes(password) };
		std::string new_password{bitsToString(password_bytes)};
		CryptoPP::SecByteBlock key((const uint8_t*)(new_password.c_str()), KEY_LENGTH); // Pass in an unsigned char array pointer, of length KEY_LENGTH. Unsigned char just means a byte of data without a bit used for sign. A.K.A. a character.

		CryptoPP::EAX<CryptoPP::AES>::Encryption encryptor;
		encryptor.SetKeyWithIV(key, key.size(), m_iv, m_iv.size());

		std::string ciphertext{""};
		CryptoPP::StringSource ss(plaintext, true, new CryptoPP::AuthenticatedEncryptionFilter(encryptor, new CryptoPP::StringSink(ciphertext)));

		// "Tamper"
		ciphertext[0] ^= 0x01;

		std::string cipher_string{""};
		CryptoPP::StringSource rs(ciphertext, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(cipher_string)));

		std::string iv_string{""};
		CryptoPP::HexEncoder iv_enc(new CryptoPP::StringSink(iv_string));
		iv_enc.Put(m_iv, m_iv.size());

		std::string return_string{""};
		return_string += iv_string + cipher_string;

		m_iv = generateIV(); // Create a new IV to enhance security.
		return return_string;
	}

};
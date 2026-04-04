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
#include <vector>
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
constexpr size_t HEXADECIMAL{16};
constexpr size_t MAX_CHAR_VALUE{255};
constexpr size_t KEY_LENGTH{CryptoPP::AES::DEFAULT_KEYLENGTH};

// Set to true to enable debug mode. Prints results into terminal.
constexpr bool DEBUG_MODE{false};

class EncDec
{
	private:
	CryptoPP::SecByteBlock m_iv;

	// Generates first 256 prime numbers (excl. 1)
	std::array<size_t, MAX_CHAR_VALUE + 1> generatePrimes()
	{
		std::array<size_t, MAX_CHAR_VALUE + 1> primes{};
		size_t prime_count{ 0 };
		size_t prime{ 0 };
		while (prime_count < MAX_CHAR_VALUE + 1)
		{
			prime++;
			size_t n{ 2 };
			bool is_prime{ true };
			while (n < prime)
			{
				if (prime % n == 0) {
					is_prime = false;
					break;
				}
				n++;
			}
			if (is_prime)
			{
				if (prime == 1) { continue; }
				primes[prime_count] = prime;
				prime_count++;
			}
		}
		return primes;
	};

	/*******************************************************************************\
	\*******************************************************************************/

	public:
	explicit EncDec() {}

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

			//if (DEBUG_MODE) std::cout << std::format("Looking at char {} : [{}]\n", remaining_string_char, remaining_string_index);

			// Keep looping until we have covered a full byte of data
			do
			{
				size_t char_bit_index{ bit_index - ((remaining_string_index * BYTE_SIZE) % total_bits) };
				//if (DEBUG_MODE) std::cout << std::format("Accessing key index: {}, value: {}. Accessing char index: {}, value: {}. XOR result: ", bit_index, int(bits[bit_index]), char_bit_index, int(char_bits[char_bit_index]));
				bits.set(bit_index, bits[bit_index] ^ char_bits[char_bit_index]);
				//if (DEBUG_MODE) std::cout << bits[bit_index] << std::endl;
				bit_index++;
			} while (bit_index % BYTE_SIZE != 0);
		}

		if (DEBUG_MODE) std::cout << std::format("Resulting bits: {}\n\n\n\n\n", bits.to_string()) << std::endl;

		return bits;
	}

	std::string bitsToString(const std::bitset<BYTE_SIZE* KEY_LENGTH>& bits)
	{
		std::string string_out{ "" };

		for (size_t byte_index{ 0 }; byte_index < KEY_LENGTH; byte_index++)
		{
			size_t bit_index{ 0 };
			int char_value{ 0 };
			do
			{
				char_value += bits[byte_index * BYTE_SIZE + bit_index] * int(pow(2, bit_index));
				bit_index++;
			} while (bit_index % BYTE_SIZE != 0);
			string_out += char(char_value);
		}

		return string_out;
	}

	CryptoPP::SecByteBlock stringToKey(const std::string& password)
	{
		// Truncate password
		std::bitset<BYTE_SIZE* KEY_LENGTH> password_bits{generateBytes(password)};
		std::string key_string{bitsToString(password_bits)};
		CryptoPP::SecByteBlock key((const uint8_t*)(key_string.c_str()), KEY_LENGTH); // Pass in an unsigned char array pointer, of length KEY_LENGTH. Unsigned char just means a byte of data without a bit used for sign. A.K.A. a character.
		return key;
	}

	/**
	* @brief A function that returns a vector of positions genereted by a seed.
	*/
	std::vector<size_t> generateIndices(const std::string& seed, const size_t& data_length)
	{
		auto charValues([](const std::string& data, std::vector<size_t>& return_vector){
			for(const char& c : data)
			{
				return_vector.push_back(size_t(c));
			}
			});

		std::vector<size_t> seed_char_values{};
		std::vector<size_t> return_vector{};
		charValues(seed, seed_char_values);
		std::array<size_t, MAX_CHAR_VALUE + 1> primes{generatePrimes()};
		size_t seed_index{0};
		for(size_t i{0}; i < data_length; i++)
		{
			size_t char_value{0}, prime_value{0};
			char_value = seed_char_values[seed_index];
			prime_value = primes[char_value];
			return_vector.push_back(prime_value);
			seed_index++;
			if(seed_index == seed.length()) { seed_index = 0; }
		}

		return return_vector;
	}

	/**
	* @brief A function that uses the password to determine where to slot in characters from the splice string into the host string. Used to hide iv.
	*/
	std::string spliceWithPassword(const std::string& password, const std::string& host_string, const std::string& splice_string)
	{
		std::vector<size_t> indices{generateIndices(password, splice_string.length())};
		std::string return_string{host_string};
		size_t next_index{0};
		size_t splice_string_index{0};
		while(!indices.empty())
		{
			next_index = indices.back();
			next_index %= return_string.length();
			indices.pop_back();

			std::string front_string{return_string.substr(0, next_index)};
			std::string back_string{return_string.substr(next_index, return_string.length() - next_index)};
			return_string = "";
			return_string += front_string + splice_string[splice_string_index] + back_string;
			splice_string_index++;
		}

		return return_string;
	}

	/**
	* @brief A function that undoes the spliceWithPassword function. Returns the value that was spliced into the host string. Alters input data string to revert.
	*/
	std::string combineFromPassword(const std::string& password, std::string& data, const size_t splice_string_length)
	{
		std::vector<size_t> indices{generateIndices(password, splice_string_length)};
		std::string splice_string{""};
		for(size_t i{0}; i < indices.size(); i++)
		{
			size_t index{indices[i]};
			index %= data.length() - 1;
			splice_string += data[index];
			data.erase(index, 1);
		}

		std::string reversed_string{""};

		for(int i{int(splice_string.length() - 1)}; i >= 0; --i)
		{
			reversed_string += splice_string[i];
		}

		return reversed_string;
	}

	std::string encrypt(const std::string& password, const std::string& plaintext)
	{
		if(DEBUG_MODE)
		{
			std::cout << std::format("Encrypting message: {}, with password: {}\n", plaintext, password);
		}

		m_iv = generateIV(); // Create a new IV to enhance security.

		CryptoPP::SecByteBlock key{stringToKey(password)};

		CryptoPP::EAX<CryptoPP::AES>::Encryption encryptor;
		encryptor.SetKeyWithIV(key, key.size(), m_iv, m_iv.size());

		std::string ciphertext{""};
		CryptoPP::StringSource ss(plaintext, true, new CryptoPP::AuthenticatedEncryptionFilter(encryptor, new CryptoPP::StringSink(ciphertext)));

		// Tamper
		ciphertext[0] ^= 0x01;

		std::string cipher_string{""};
		CryptoPP::StringSource rs(ciphertext, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(cipher_string)));

		std::string iv_string{""};
		CryptoPP::HexEncoder iv_enc(new CryptoPP::StringSink(iv_string));
		iv_enc.Put(m_iv, m_iv.size());

		std::string return_string{""};
		return_string = spliceWithPassword(password, cipher_string, iv_string);

		if(DEBUG_MODE)
		{
			std::cout << std::format("IV Hex: {}\nMessage Hex: {}\nResulting Hex: {}\n", iv_string, cipher_string, return_string);
		}
		
		return return_string;
	}

	std::string decrypt(const std::string& password, const std::string& ciphertext)
	{
		if(DEBUG_MODE)
		{
			std::cout << std::format("Decrypting message: {}, with password: {}\n", ciphertext, password);
		}

		CryptoPP::SecByteBlock key{stringToKey(password)};

		std::string ciphertext_hex{ciphertext};
		// Retrieve the IV from the ciphertext. Every char is represented as two hexadecimal values.
		std::string iv_hex{combineFromPassword(password, ciphertext_hex, HEXADECIMAL * 2)};
		std::string iv_str{""};
		CryptoPP::StringSource ss_iv(iv_hex, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(iv_str)));
		CryptoPP::SecByteBlock iv((const uint8_t*)(iv_str.c_str()), KEY_LENGTH);

		CryptoPP::EAX<CryptoPP::AES>::Decryption decryptor;
		decryptor.SetKeyWithIV(key, key.size(), iv, iv.size());
		std::string encrypted_message{""};
		CryptoPP::StringSource ss_msg(ciphertext_hex, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(encrypted_message)));

		encrypted_message[0] ^= 0x01;
		
		std::string plaintext{""};
		CryptoPP::StringSource ss(encrypted_message, true, new CryptoPP::AuthenticatedDecryptionFilter(decryptor, new CryptoPP::StringSink(plaintext)));

		if(DEBUG_MODE)
		{
			std::cout << std::format("Resulting message: {}\n", plaintext);
		}
		
		return plaintext;
	}
};
/**
 * This program is to be used as an abstraction layer to more easily interact with the necessary components of Crypto++
 * 
 * https://stackoverflow.com/questions/33753293/using-stdstring-for-key-with-aes-encryption-in-crypto
 * https://www.cryptopp.com/wiki/Advanced_Encryption_Standard
 * IV is to be stored with ciphertext -> RNG
 */
#pragma once
#include <iostream>
#include <iomanip>

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

using namespace std;

CryptoPP::byte generateBytes(string input);

// Testing functionality
int main()
{
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    memset(iv, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
    
    for(size_t byte_index{0}; byte_index < CryptoPP::AES::DEFAULT_KEYLENGTH; byte_index++)
    {
        cout << int(key[byte_index]) << " ";
    }

    return 0;
}

CryptoPP::byte generateBytes(string input)
{

}
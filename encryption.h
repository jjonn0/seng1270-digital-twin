#pragma once
#include <string>
#include <bitset>
#include <array>
#include <iostream>

/**
 * @brief Converts a string into a 4x4 block of bytes used as a key
 */
std::string generateKey(std::string key_data);
std::string encrypt(std::string key, std::string data);

std::string generateKey(std::string key_data)
{
    const size_t CHAR_SIZE{8};
    std::string bits{""};
    for(char c : key_data)
    {
        for(int bit_index{CHAR_SIZE - 1}; bit_index >= 0; --bit_index)
        {
            std::cout << bit_index << " " << ((c >> bit_index) & 1) << std::endl;
            int bit{(c >> bit_index) & 1};
            bits += "" + bit;
        }
        std::cout << bits << " end" << std::endl;
    }

    return "";
}

std::string encrypt(std::string key, std::string data)
{
    return "";
}
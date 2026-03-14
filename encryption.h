#pragma once
#include <string>
#include <array>
#include <iostream>
#include <cmath>

/**
 * @brief           Swaps a plaintext string with a binary string and vice versa.
 * @param data      A string that is either plaintext or binary.
 * @param invert    If false, takes a plaintext string and returns a binary string. Of tru, takes a binary string and returns a plaintext string.
 */
std::string binarySwitch(std::string data, bool invert);
/** @warning Not implemented */
std::string generateKey(std::string key_data);
/** @warning Not implemented */
std::string encrypt(std::string key, std::string data);

std::string binarySwitch(std::string data, bool invert = false)
{
    const size_t CHAR_SIZE{8};
    std::string return_string{""};
    // Binary-to-string conversion
    if(invert)
    {
        size_t char_value{0};
        for(int char_index{0}; char_index <= data.length(); char_index++)
        {
            int bit_index{char_index % int(CHAR_SIZE)};
            if(bit_index == 0 && char_index != 0)
            {
                return_string += char(char_value);
                char_value = 0;
            }
            int bit_state{data[char_index] == '0' ? 0 : 1};
            char_value += bit_state * pow(2, CHAR_SIZE - bit_index - 1);
        }
    }
    // String-to-binary conversion
    else
    {
        for(char c : data)
        {
            for(int bit_index{CHAR_SIZE - 1}; bit_index >= 0; bit_index--)
            {
                int bit{(c & (1 << bit_index)) ? '1' : '0'};
                return_string += bit;
            }
        }
    }
    
    return return_string;
}

std::string generateKey(std::string key_data)
{
    return "";
}

std::string encrypt(std::string key, std::string data)
{
    return "";
}
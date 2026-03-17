#pragma once
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <bitset>
#include <cmath>

static const size_t COL_COUNT{4}; // Number of cols contained in the block
static const size_t ROW_COUNT{4}; // Number of rows contained in the block
static const size_t BYTE_SIZE{8}; // Number of bits in a byte
static const size_t BLOCK_SIZE{COL_COUNT * ROW_COUNT}; // Number of cells in a block
static const size_t KEY_SIZE{BLOCK_SIZE * BYTE_SIZE}; // Key size in bits

void displayChars(std::vector<size_t> numbers);
/** @warning Not implemented */
std::string encrypt(std::string key, std::string data);

void displayChars(std::vector<size_t> numbers)
{
    for(size_t n : numbers)
    {
        std::cout << char(n);
    }
}

std::string encrypt(size_t key, std::string data)
{
    
    return "";
}

class ByteBlock
{
    private:
    
    std::array<std::array<std::bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> m_byte_block;

    public:
    ByteBlock(const std::array<std::bitset<BYTE_SIZE>, BLOCK_SIZE>& values) 
    {
        for(size_t index{0}; index < values.size(); index++)
        {
            size_t row_index{size_t(floor(index / COL_COUNT))};
            m_byte_block[row_index][index - row_index * COL_COUNT] = values[index];
        }
    }

    void displayBlock()
    {
        for(std::array<std::bitset<BYTE_SIZE>, ROW_COUNT> row : m_byte_block)
        {
            for(std::bitset<BYTE_SIZE> bit_block : row)
            {
                std::cout << bit_block << " ";
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief   Shifts the rows up by the given amount.
     */
    void shiftRows(size_t shift_amount)
    {
        std::array<std::array<std::bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> pre_shift_block{m_byte_block};

        for(size_t row_index{0}; row_index < ROW_COUNT; row_index++)
        {
            m_byte_block[row_index] = pre_shift_block[((row_index + shift_amount) % ROW_COUNT)];
        }
    }
    /**
     * @brief   Shifts the columns left by the given amount.
     */
    void shiftCols(size_t shift_amount)
    {
        std::array<std::array<std::bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> pre_shift_block{m_byte_block};

        for(size_t row_index{0}; row_index < ROW_COUNT; row_index++)
        {
            for(size_t col_index{0}; col_index < COL_COUNT; col_index++)
            {
                m_byte_block[row_index][col_index] = pre_shift_block[row_index][((col_index + shift_amount) % COL_COUNT)];
            }
        }
    }
};

class EncryptionKey
{
    private:
    std::bitset<KEY_SIZE> m_key;

    public:
    EncryptionKey(std::string data)
    {
        size_t byte_index{0};
        for(const char c : data)
        {
            std::bitset<BYTE_SIZE> char_bits{unsigned long long int(c)};
            for(size_t key_bit_index{0}; key_bit_index < KEY_SIZE; key_bit_index++)
            {
                
            }
        }
    }

    std::bitset<KEY_SIZE> getKey() { return m_key; }

    std::string toString() { return m_key.to_string(); }
};
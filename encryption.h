#pragma once
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <bitset>
#include <cmath>

void displayChars(vector<size_t> numbers);
/** @warning Not implemented */
std::string encrypt(std::string key, std::string data);

void displayChars(vector<size_t> numbers)
{
    for(size_t n : numbers)
    {
        cout << char(n);
    }
}

std::string encrypt(size_t key, std::string data)
{
    
    return "";
}

class ByteBlock
{
    private:
    static const size_t COL_COUNT{4};
    static const size_t ROW_COUNT{4};
    static const size_t BYTE_SIZE{8};
    static const size_t BLOCK_SIZE{COL_COUNT * ROW_COUNT};
    array<array<bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> m_byte_block;

    public:
    ByteBlock(const array<bitset<BYTE_SIZE>, BLOCK_SIZE>& values) 
    {
        for(size_t index{0}; index < values.size(); index++)
        {
            size_t row_index{size_t(floor(index / COL_COUNT))};
            m_byte_block[row_index][index - row_index * COL_COUNT] = values[index];
        }
    }

    void displayBlock()
    {
        for(array<bitset<BYTE_SIZE>, ROW_COUNT> row : m_byte_block)
        {
            for(bitset<BYTE_SIZE> bit_block : row)
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
        array<array<bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> pre_shift_block{m_byte_block};

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
        array<array<bitset<BYTE_SIZE>, ROW_COUNT>, COL_COUNT> pre_shift_block{m_byte_block};

        for(size_t row_index{0}; row_index < ROW_COUNT; row_index++)
        {
            for(size_t col_index{0}; col_index < COL_COUNT; col_index++)
            {
                m_byte_block[row_index][col_index] = pre_shift_block[row_index][((col_index + shift_amount) % COL_COUNT)];
            }
        }
    }
};
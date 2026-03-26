/** @warning Not completed */
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "rapidcsv.h"

enum FileType{
        DAT,
        CSV
    };

class FileHandler
{
    private:
    std::string file_name;
    FileType file_type;
    std::fstream text_file_stream;

    void pushError(std::string error_message)
    {
        std::cerr << "\033[91m" + std::string("FileHandler encountered an error: ") + error_message + "\033[97m" << std::endl;
    }
    
    public:
    FileHandler(const FileType file_type, const std::string file_name)
    {
        this->file_type = file_type;
        this->file_name = file_name;
    }
    void writeToFile(std::string input_data)
    {
        if(file_type == FileType(DAT) && !text_file_stream)
        {
            text_file_stream = std::fstream(file_name);
        }

        switch(file_type)
        {
            case 0:

                break;
            case 1:
                break;
            default:
                pushError("No valid file type chosen.");
        }
    }
    void closeFile()
    {
        if(text_file_stream) text_file_stream.clear();
    }
};
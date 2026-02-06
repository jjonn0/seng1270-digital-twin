#pragma once
#include <iostream>
#include <string>

class Person
{
    private:
    std::string name;

    public:
    explicit Person(std::string name = "") : name{name} {};
    std::string getName()
    {
        return name;
    }
    void setName(std::string name)
    {
        this->name = name;
    }
};
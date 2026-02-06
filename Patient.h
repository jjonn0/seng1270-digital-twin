#pragma once
#include "Person.h"
#include <iostream>
#include <string>

class Patient : Person
{
    private:
    size_t age;
    std::string reason_of_admission;

    public:
    explicit Patient(size_t age = 0, std::string reason_of_admission = "") : age{age}, reason_of_admission{reason_of_admission} {}
    size_t getAge()
    {
        return age;
    }
    std::string getReasonOfAdmission()
    {
        return reason_of_admission;
    }
    void setAge(size_t age)
    {
        this->age = age;
    }
    void setReasonOfAdmission(std::string reason_of_admission)
    {
        this->reason_of_admission = reason_of_admission;
    }
};
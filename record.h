#pragma once
#include <string>
#include "timereference.h"
#include "timetracker.h"

class Record
{
    private:
    std::string name;
    size_t age;
    TimeReference time_of_admission;
    TimeTracker expected_time_of_stay;

    public:
    explicit Record() {}
    Record(size_t *seconds_since_unix_ptr, std::string name = "", size_t age = 0, size_t expected_time_of_stay = 0)
    {
        this->name = name;
        this->age = age;
        time_of_admission = TimeReference(seconds_since_unix_ptr);
        this->expected_time_of_stay = TimeTracker(seconds_since_unix_ptr, expected_time_of_stay);
    }
    void setName(std::string name)
    {
        this->name = name;
    }
    void setAge(size_t age)
    {
        this->age = age;
    }

    // TO-DO
    // Add to timereference.h
    // -> Function to go from a readable time to unix time
    //      - ss/mm/hh/dd/MM/YYYY
};
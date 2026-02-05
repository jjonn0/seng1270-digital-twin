#pragma once
#include <iostream>
#include <string>

class Time
{
    private:
        int hours;
        int minutes;
        int seconds;
    
    public:
        Time(int hours = 0, int minutes = 0, int seconds = 0)
        {
            this->hours = hours;
            this->minutes = minutes;
            this->seconds = seconds;
        }
        
};
#pragma once
#include <iostream>
#include <string>
#include <chrono>

class Time
{
    private:
        size_t seconds_since_epoch_to_creation;
        int seconds_until_expiration;
        bool expired;
    
    public:
    /**
     * @param seconds_since_epoch : Seconds that have elapsed since UNIX epoch. Read @see for information on how to get UNIX epoch.
     * @param seconds_until_expiration : Seconds that need to elapse until time is considered expired.
     * 
     * @see To get UNIX epoch in seconds:
     * chrono::time_point time_point = chrono::system_clock::now();
     * int seconds_since_epoch = chrono::duration_cast<chrono::seconds>(time_point.time_since_epoch()).count();
     */
    Time(size_t seconds_since_epoch, int seconds_until_expiration = 0)
    {
        seconds_since_epoch_to_creation = seconds_since_epoch;
        this->seconds_until_expiration = seconds_until_expiration;
        expired = (seconds_until_expiration <= 0 ? true : false);
    }
    void updateTime(size_t seconds_since_epoch)
    {
        seconds_until_expiration = seconds_until_expiration + seconds_since_epoch_to_creation - seconds_since_epoch;
        expired = (seconds_until_expiration <= 0 ? true : false);
    }
    int getRemainingSeconds()
    {
        return seconds_until_expiration;
    }
};
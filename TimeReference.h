/**
 * @class TimeReference
 * @brief Allows for tracking of time.
 * @author jjonn0
 */

#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <cmath>

class TimeReference
{
    private:
    size_t* seconds_since_unix_ptr;
    size_t creation_since_unix;
    size_t expiration_since_unix;
    
    public:
    /**
     * @param creation_since_unix_ptr : A pointer that points to seconds that have elapsed since UNIX epoch.
     * @param seconds_until_expiration : Seconds that need to elapse until time is considered expired.
     */
    TimeReference(size_t * seconds_since_unix_ptr, size_t seconds_until_expiration = 0)
    {
        this->seconds_since_unix_ptr = seconds_since_unix_ptr;
        this->creation_since_unix = *seconds_since_unix_ptr;
        expiration_since_unix = *seconds_since_unix_ptr + seconds_until_expiration;
    }
    int getRemainingSeconds()
    {
        return expiration_since_unix - *seconds_since_unix_ptr;
    }
    std::string getTimeStamp()
    {
        auto floor_division([](auto x, auto y){ return int(floor(x / y)); });
        int remaining_seconds{getRemainingSeconds()};
        remaining_seconds = (remaining_seconds == 0 ? 0 : remaining_seconds);
        int remaining_days{floor_division(remaining_seconds, 86400)};
        remaining_seconds -= remaining_days * 86400;
        int remaining_hours{floor_division(remaining_seconds, 3600)};
        remaining_seconds -= remaining_hours * 3600;
        int remaining_minutes{floor_division(remaining_seconds, 60)};
        remaining_seconds -= remaining_minutes * 60;

        return std::format("{:0>2d}d:{:0>2d}h:{:0>2d}m:{:0>2d}s", remaining_days, remaining_hours, remaining_minutes, remaining_seconds);
    }
    void setRemainingSeconds(size_t seconds_until_expiration)
    {
        expiration_since_unix = creation_since_unix + seconds_until_expiration;
    }
    void addRemainingSeconds(size_t additional_seconds_until_expiration)
    {
        expiration_since_unix += additional_seconds_until_expiration;
    }
    // Whether or not the time reference endpoint has been passed.
    bool isPassed()
    {
        return (getRemainingSeconds() < 0 ? true : false);
    }
};

/**
 * @brief Takes in a pointer utilized by TimeReference objects in order to update the time globally.
 * @param unix_ptr : A pointer that points to a size_t variable that has UNIX time assigned to it.
 */
void update_seconds_since_unix(size_t * unix_ptr)
{
    std::chrono::time_point time_point = std::chrono::system_clock::now();
    size_t seconds_since_unix = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    *unix_ptr = seconds_since_unix;
}
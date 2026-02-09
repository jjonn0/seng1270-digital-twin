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
#include <regex>

class TimeReference
{
    protected:
    size_t* seconds_since_unix_ptr;
    size_t creation_since_unix;
    
    public:
    /**
     * @brief                           Creates a TimeReference object that does not have an expiration.
     * @param creation_since_unix_ptr   A pointer that points to seconds that have elapsed since UNIX epoch.
     */
    explicit TimeReference() {}
    explicit TimeReference(size_t *seconds_since_unix_ptr) : seconds_since_unix_ptr{seconds_since_unix_ptr} {}
    // Return current UNIX time in seconds
    size_t getCurrentSeconds()
    {
        return *seconds_since_unix_ptr;
    }
    size_t getCreationSeconds()
    {
        return creation_since_unix;
    }
};

/**
 * @brief           Takes in a pointer utilized by TimeReference objects in order to update the time globally.
 * @param unix_ptr  A pointer that points to a size_t variable that has UNIX time assigned to it.
 */
void update_seconds_since_unix(size_t *unix_ptr)
{
    std::chrono::time_point time_point = std::chrono::system_clock::now();
    size_t seconds_since_unix = std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
    *unix_ptr = seconds_since_unix;
}
/**
 * @brief           Takes in a timestamp of the following format    ss:mm:hh:DD:MM:YYYY    Returns UNIX time in seconds.
 * @param timestamp A timestamp used to translate to UNIX time.
 */
size_t get_unix_time(std::string timestamp)
{
    std::string parser{""};

    parser = timestamp.substr(timestamp.size() - 5, timestamp.size() - 1);

    return 0;
}
#include <iostream>
#include <vector>
#include <cmath>
#include "profiles.h"

enum PatientStatus
{
    DISCHARGED,
    ADMITTED
};

// Units of time based on seconds
enum TimeUnit
{
    HOUR = 60 * 60,
    DAY = HOUR * 24,
    WEEK = DAY * 7,
    YEAR = WEEK * 52
};

/// @brief  Uses a tm struct to create a timestamp based on given parameters.
/// @return Returns a time_t object.
time_t getTimestamp(size_t year, size_t month, size_t month_day, size_t hour, size_t minute, size_t second);
time_t getTimestamp(size_t year, size_t month, size_t month_day);

/// @brief                  Takes in a patient profile, and two time points, and calculates if the patient has been discharged or not.
/// @param patient_profile  The associated patient profile.
/// @param previous_time    The previous timestamp used.
/// @param current_time     The currently used timestamp.
/// @return                 PatientStatus::DISCHARGED or PatientStatus::ADMITTED
PatientStatus patientAdmissionStatus(PatientProfile patient_profile, time_t previous_time, time_t current_time);

/// @brief                  Takes in a patient profile, cost per elapsed time unit, elapsed time, and the unit of time. Returns an estimated cost based on params.
/// @param cost_per_time    The cost per unit time.
/// @param elapsed_time     The elapsed time. (In seconds.)
/// @param time_unit        The unit of time.
/// @return                 An estimated cost multiplied by 100. (To avoid floating-point precision errors.)
/// @note                   Rounds up for some calculations.
size_t getCostEstimation(size_t cost_per_time, time_t elapsed_time, TimeUnit time_unit);

std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> getUpcomingShifts(StaffProfile staff_profile, time_t current_time);

//*****************************************************************************************//
//*****************************************************************************************//

time_t getTimestamp(size_t year, size_t month, size_t month_day, size_t hour, size_t minute, size_t second)
{
    struct tm datetime;
    datetime.tm_year = year - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = month_day;
    datetime.tm_hour = hour;
    datetime.tm_min = minute;
    datetime.tm_sec = second;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

time_t getTimestamp(size_t year, size_t month, size_t month_day)
{
    struct tm datetime;
    datetime.tm_year = year - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = month_day;
    datetime.tm_hour = 0;
    datetime.tm_min = 0;
    datetime.tm_sec = 0;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

PatientStatus patientAdmissionStatus(PatientProfile patient_profile, time_t previous_time, time_t current_time)
{
    time_t discharge_time{patient_profile.getExpectedTimeOfStay()};
    bool before_previous{(discharge_time <= previous_time) ? true : false};
    bool before_current{(discharge_time <= current_time) ? true : false};

    if((!before_previous && before_current) || (before_previous && before_current)) { return PatientStatus::DISCHARGED; }

    return ADMITTED;
}

size_t getCostEstimation(size_t cost_per_time, time_t elapsed_time, TimeUnit time_unit)
{
    size_t elapsed_time_units{size_t(ceil(elapsed_time / time_unit))};
    return cost_per_time * elapsed_time_units;
}

std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> getUpcomingShifts(StaffProfile staff_profile, time_t current_time)
{
    std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> upcoming_shifts{};
    std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> staff_shifts{staff_profile.getShifts()};
    size_t upcoming_count{0};

    upcoming_shifts.fill(TimeBlock{0, 0});
    for(TimeBlock shift : staff_shifts)
    {
        if(shift.time_block_start > current_time)
        {
            upcoming_shifts[upcoming_count] = shift;
            upcoming_count++;
        }
    }

    return upcoming_shifts;
}
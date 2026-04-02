#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <typeinfo>
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

class Room
{
    private:
    const std::string m_room_number;
    size_t m_max_occupancy;
    std::vector<PatientProfile> m_patients;
    std::vector<StaffProfile> m_staff;

    template<typename T>
    bool doesProfileExist(const T& profile)
    {
        size_t profile_number = profile.getProfileNumber();
        if (typeid(T) == typeid(StaffProfile))
        {
            for (StaffProfile s : m_staff)
            {
                if (s.getProfileNumber() == profile_number) { return true; }
            }
        }
        else if (typeid(T) == typeid(PatientProfile))
        {
            for (PatientProfile p : m_patients)
            {
                if (p.getProfileNumber() == profile_number) { return true; }
            }
        }
        return false;
    }

    template<typename T>
    void addProfileToVector(T profile, std::vector<T>& vector)
    {
        if(doesProfileExist(profile)) { std::cerr << std::format("Profile ID {} already exists.\n", profile.getProfileNumber()); }
        else { vector.push_back(profile); }
    }

    template<typename T>
    bool removeProfileFromVector(T profile, std::vector<T>& vector)
    {
        for (size_t profile_index{ 0 }; profile_index < vector.size(); profile_index++)
        {
            if (vector[profile_index].getProfileNumber() == profile.getProfileNumber())
            {
                vector.erase(vector.begin() + profile_index);
                return true;
            }
        }
        std::cerr << std::format("Profile ID {} was not found!\n", profile.getProfileNumber());
        return false;
    }

    public:
    Room(std::string room_number, size_t max_occupancy = 0, std::vector<PatientProfile> patients = {}, std::vector<StaffProfile> staff = {}) : m_room_number{room_number}, m_max_occupancy{max_occupancy}, m_patients{patients}, m_staff{staff} {}

    void setMaxOccupancy(const size_t& max_occupancy) { m_max_occupancy = max_occupancy; }
    size_t getMaxOccupancy() { return m_max_occupancy; }

    void setPatients(const std::vector<PatientProfile> patients)
    { 
        for(const PatientProfile p : patients)
        {
            m_patients.push_back(p);
        }
    }
    std::vector<PatientProfile> getPatients() { return m_patients; }

    void setStaff(const std::vector<StaffProfile> staff)
    { 
        for (const StaffProfile s : staff)
        {
            m_staff.push_back(s);
        }
    }
    std::vector<StaffProfile> getStaff() { return m_staff; }

    void addPatientProfile(PatientProfile& profile) { addProfileToVector(profile, m_patients); }
    void removePatientProfile(PatientProfile& profile) { removeProfileFromVector(profile, m_patients); }

    void movePatient(PatientProfile& patient_to_move, Room& new_room)
    {
        if(removeProfileFromVector(patient_to_move, m_patients))
        {
            new_room.addPatientProfile(patient_to_move);
        }
    }

    void addStaffProfile(const StaffProfile& profile) { addProfileToVector(profile, m_staff); }
    void removeStaffProfile(StaffProfile& profile) { removeProfileFromVector(profile, m_staff); }

    void moveStaff(StaffProfile& staff_to_move, Room& new_room)
    {
        if (removeProfileFromVector(staff_to_move, m_staff))
        {
            new_room.addStaffProfile(staff_to_move);
        }
    }

    size_t getCurrentOccupancy() { return m_patients.size(); }
    
    std::string getOccupancyStatus() { return std::format("{}/{}", getCurrentOccupancy(), m_max_occupancy); }
};
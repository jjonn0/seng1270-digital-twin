#pragma once
#include <string>
#include <ctime>
#include <format>
#include "profile.h"

class StaffProfile : public Profile
{
    private:
    std::string m_role; // The role the staff member has.
    size_t m_wage;      // The wage the staff member recieves. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.

    public:
    // New profile constructors
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     */
    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age) : Profile(profile_number, first_name, last_name, age) {}
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the staff profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param role              The role that the staff member has.
     * @param wage              The wage the staff member is payed by the hour. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.
     */
    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, std::string role, size_t wage) : Profile(profile_number, first_name, last_name, age), m_role{role}, m_wage{wage} {}

    // Profile constructors for loading profiles from disk.
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the staff profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param creation_date     The date that the original staff profile was created on.
     */
    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, time_t creation_date) : Profile(profile_number, first_name, last_name, age, creation_date) {}
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the staff profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param creation_date     The date that the original staff profile was created on.
     * @param role              The role that the staff member has.
     * @param wage              The wage the staff member is payed by the hour. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.
     */
    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, time_t creation_date, std::string role, size_t wage) : Profile(profile_number, first_name, last_name, age, creation_date), m_role{role}, m_wage{wage} {}
    ~StaffProfile() {}

    std::string getRole() { return m_role; }
    size_t getWage() { return m_wage; }
    std::string toString() { return std::format("{} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_role, m_wage); }

    void setRole(std::string role) { m_role = role; }
    void setWage(size_t wage) { m_wage = wage; }
};
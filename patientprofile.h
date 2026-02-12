#pragma once
#include <string>
#include <ctime>
#include "profile.h"

class PatientProfile : public Profile
{
    private:
    std::string m_reason_of_admission;  // The reason the patient is admitted to the hospital.
    time_t m_time_of_admission;         // The time at which the patient was processed.
    time_t m_expected_time_of_stay;     // The time at which the patient is expected to have been discharged.

    public:
    // New profile constructors
    /**
     * @brief                   Default constructor for making a brand new patient profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     */
    PatientProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0) : Profile(profile_number, first_name, last_name, age) {}

    // Profile constructors for loading profiles from disk.
    /**
     * @brief                   Default constructor for making a brand new patient profile.
     * @param profile_number    A number used to reference the patient profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param creation_date     The date that the original patient profile was created on.
     */
    PatientProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age, time_t creation_date) : Profile(profile_number, first_name, last_name, age, creation_date) {}
    /**
     * @brief                           Default constructor for making a brand new patient profile.
     * @param profile_number            A number used to reference the patient profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param creation_date             The date that the original patient profile was created on.
     * @param reason_of_admission       The reason the patient is admitted to the hospital.
     * @param m_time_of_admission       The time at which the patient was processed.
     * @param m_expected_time_of_stay   The time at which the patient is expected to have been discharged.
     */
    PatientProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age, time_t creation_date, std::string reason_of_admission, time_t time_of_admission, time_t expected_time_of_stay) : 
    Profile(profile_number, first_name, last_name, age, creation_date), m_reason_of_admission{reason_of_admission}, m_time_of_admission{time_of_admission}, m_expected_time_of_stay{expected_time_of_stay} {}
    ~PatientProfile() {}

    std::string getReasonOfAdmission() { return m_reason_of_admission; }
    time_t getTimeOfAdmission() { return m_time_of_admission; }
    time_t getExpectedTimeOfStay() { return m_expected_time_of_stay; }
    std::string toString() { return std::format("{} {} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_reason_of_admission, m_time_of_admission, m_expected_time_of_stay); }

    void setReasonOfAdmission(std::string reason_of_admission) { m_reason_of_admission = reason_of_admission; }
    void setTimeOfAdmission(time_t time_of_admission) { m_time_of_admission = time_of_admission; }
    void setExpectedTimeOfStay(time_t expected_time_of_stay) { m_expected_time_of_stay = expected_time_of_stay; }
};
#pragma once
#include <string>
#include <ctime>
#include "profile.h"

class PatientProfile : public Profile
{
    private:
    std::string m_reason_of_admission; // The reason the patient is admitted to the hospital.
    time_t m_time_of_admission; // The time at which the patient was processed.
    time_t m_expected_time_of_stay; // The time at which the patient is expected to have been discharged.

    public:
    /**
     * @brief                   Default constructor for making a brand new patient profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param name              The name of the patient on profile.
     * @param age               The age of the patient on profile.
     */
    PatientProfile(size_t profile_number, std::string name = "<unknown>", size_t age = 0) : Profile(profile_number, name, age) {}
    /**
     * @brief                   Default constructor for making a brand new patient profile.
     * @param profile_number    A number used to reference the patient profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param name              The name of the patient on profile.
     * @param age               The age of the patient on profile.
     * @param m_creation_date   The date that the original patient profile was created on.
     */
    PatientProfile(size_t profile_number, std::string name, size_t age, time_t creation_date) : Profile(profile_number, name, age, creation_date) {}
    ~PatientProfile() {}

    std::string getReasonOfAdmission() { return m_reason_of_admission; }
    time_t getTimeOfAdmission() { return m_time_of_admission; }
    time_t getExpectedTimeOfStay() { return m_expected_time_of_stay; }

    void setReasonOfAdmission(std::string reason_of_admission) { m_reason_of_admission = reason_of_admission; }
    void setTimeOfAdmission(time_t time_of_admission) { m_time_of_admission = time_of_admission; }
    void setExpectedTimeOfStay(time_t expected_time_of_stay) { m_expected_time_of_stay = expected_time_of_stay; }
};
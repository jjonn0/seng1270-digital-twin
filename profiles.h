#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <format>

class Profile
{
    protected:
    std::string m_first_name;   // The first name of the person on profile
    std::string m_last_name;    // The last name of the person on profile
    size_t m_age;               // The age of the person on profile
    time_t m_creation_date;     // [Cannot be changed] The data the profile was created. This is assigned automatically, unless specified at the end of the constructor.
    size_t m_profile_number;    // [Cannot be changed] The internal profile number. This is not created automatically, and must be manually assigned during construction.
    
    void constructionMessage()
    {
        //std::cout << std::format("New profile #{} was created.\n", m_profile_number);
    }
    void destructionMessage()
    {
        //std::cout << std::format("Profile #{} was destroyed.\n", m_profile_number);
    }

    public:
    /**
     * @brief                   Default constructor for making a brand new profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     */
    Profile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0) : m_profile_number{profile_number}, m_first_name{first_name}, m_last_name{last_name}, m_age{age}
    {
        constructionMessage();
        time(&m_creation_date);
    }
    /**
     * @brief                   Default constructor for making a brand new profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     */
    Profile(size_t profile_number, std::string first_name, std::string last_name, size_t age, time_t creation_date) : m_profile_number{profile_number}, m_first_name{first_name}, m_last_name{last_name}, m_age{age}, m_creation_date{creation_date} { constructionMessage(); }
    ~Profile() { destructionMessage(); }

    std::string getFirstName() { return m_first_name; }
    std::string getLastName() { return m_last_name; }
    size_t getAge() { return m_age; }
    time_t getCreationDate() { return m_creation_date; }
    size_t getProfileNumber() { return m_profile_number; }
    std::string toString() { return std::format("{} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date); }

    void setFirstName(std::string first_name) { m_first_name = first_name; }
    void setLastName(std::string last_name) { m_last_name = last_name; }
    void setAge(size_t age) { m_age = age; }
};

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
    PatientProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0, std::string reason_of_admission = "<unknown>", time_t time_of_admission = 0, time_t expected_time_of_stay = 0) : 
    Profile(profile_number, first_name, last_name, age), m_reason_of_admission{reason_of_admission}, m_time_of_admission{time_of_admission}, m_expected_time_of_stay{expected_time_of_stay} {}

    // Profile constructors for loading profiles from disk.
    /**
     * @brief                           Default constructor for making a brand new patient profile.
     * @param profile_number            A number used to reference the patient profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     */
    PatientProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, time_t creation_date, std::string reason_of_admission, time_t time_of_admission, time_t expected_time_of_stay) : 
    Profile(profile_number, first_name, last_name, age, creation_date), m_reason_of_admission{reason_of_admission}, m_time_of_admission{time_of_admission}, m_expected_time_of_stay{expected_time_of_stay} {}

    std::string getReasonOfAdmission() { return m_reason_of_admission; }
    time_t getTimeOfAdmission() { return m_time_of_admission; }
    time_t getExpectedTimeOfStay() { return m_expected_time_of_stay; }
    std::string toString() { return std::format("{} {} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_reason_of_admission, m_time_of_admission, m_expected_time_of_stay); }
    std::string toFormattedString()
    {
        std::string profile_creation_date_string{ctime(&m_creation_date)};
        std::string time_of_admission_string{ctime(&m_time_of_admission)};
        std::string expected_time_of_stay_string{ctime(&m_expected_time_of_stay)};
        return std::format("Profile Number: {:<20d} | Last Name: {:<20s} | First Name: {:<20s} | Age: {:<3d}\nProfile Creation Date: {:<s}Reason of Admission: {:<s}\nTime of Admission: {:<s}Expected Time of Stay: {:<s}",
        m_profile_number, m_last_name, m_first_name, m_age, profile_creation_date_string, m_reason_of_admission, time_of_admission_string, expected_time_of_stay_string);
    }

    void setReasonOfAdmission(std::string reason_of_admission) { m_reason_of_admission = reason_of_admission; }
    void setTimeOfAdmission(time_t time_of_admission) { m_time_of_admission = time_of_admission; }
    void setExpectedTimeOfStay(time_t expected_time_of_stay) { m_expected_time_of_stay = expected_time_of_stay; }
};

class StaffProfile : public Profile
{
    private:
    std::string m_role; // The role the staff member has.
    size_t m_wage;      // The wage the staff member recieves. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.

    public:
    // New profile constructors
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the staff profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param wage              The wage the staff member is payed by the hour. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.
     */
    StaffProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0, std::string role = "<unknown>", size_t wage = 0) : Profile(profile_number, first_name, last_name, age), m_role{role}, m_wage{wage} {}

    // Profile constructors for loading profiles from disk.
    /**
     * @brief                   Default constructor for making a brand new staff profile.
     * @param profile_number    A number used to reference the staff profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param wage              The wage the staff member is payed by the hour. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.
     */
    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, time_t creation_date, std::string role, size_t wage) : Profile(profile_number, first_name, last_name, age, creation_date), m_role{role}, m_wage{wage} {}

    std::string getRole() { return m_role; }
    size_t getWage() { return m_wage; }
    std::string toString() { return std::format("{} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_role, m_wage); }
    std::string toFormattedString()
    {
        std::string profile_creation_date_string{ctime(&m_creation_date)};
        return std::format("Profile Number: {:<20d} | Last Name: {:<20s} | First Name: {:<20s} | Age: {:<3d}\nProfile Creation Date: {:<s}Role: {:<s}\nWage: {:<d}\n",
        m_profile_number, m_last_name, m_first_name, m_age, profile_creation_date_string, m_role, m_wage);
    }

    void setRole(std::string role) { m_role = role; }
    void setWage(size_t wage) { m_wage = wage; }
};
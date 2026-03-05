#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <format>
#include <array>
#include <vector>

const size_t MAXIMUM_STORED_SHIFTS{10};

/// @brief The unit the staff member is assigned to. Use GENERAL for a non-specific unit.
enum Unit
{
    GENERAL,
    ER,
    ICU,
    CARDIAC,
    NEURO,
    POST_OP,
    GERIATRIC,
    TRAUMA,
    NEPHRO,
    PSYCH,
    ONCOLOGY,
    PALLIATIVE
};

/// @brief The time designation of a shift. Uses two time_t data members for a start and end time.
struct TimeBlock
{
    time_t time_block_start;
    time_t time_block_end;

    size_t getTotalTime() const { return time_block_end - time_block_start; }
};

class Profile
{
    public:
    virtual std::string getFirstName() const = 0;
    virtual void setFirstName(std::string first_name) = 0;

    virtual std::string getLastName() const = 0;
    virtual void setLastName(std::string last_name) = 0;

    virtual size_t getAge() const = 0;
    virtual void setAge(size_t age) = 0;

    virtual time_t getCreationDate() const = 0;

    virtual size_t getProfileNumber() const = 0;

    virtual std::string toString() const = 0;
    virtual std::string toFormattedString() const = 0;
};

class PatientProfile final : public Profile
{
    private:
    std::string m_first_name;           // The first name of the person on profile.
    std::string m_last_name;            // The last name of the person on profile.
    size_t m_age;                       // The age of the person on profile.
    time_t m_creation_date;             // The data the profile was created. This is assigned automatically, unless specified at the end of the constructor.
    size_t m_profile_number;            // The internal profile number. This is not created automatically, and must be manually assigned during construction.
    std::string m_reason_of_admission;  // The reason the patient is admitted to the hospital.
    time_t m_time_of_admission;         // The time at which the patient was processed.
    time_t m_expected_time_of_stay;     // The time at which the patient is expected to have been discharged.

    public:
    // Creating a new patient profile
    PatientProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0, std::string reason_of_admission = "<unknown>", time_t time_of_admission = 0, time_t expected_time_of_stay = 0) : 
    m_profile_number{profile_number}, m_first_name{first_name}, m_last_name{last_name}, m_age{age}, m_reason_of_admission{reason_of_admission}, m_time_of_admission{time_of_admission}, m_expected_time_of_stay{expected_time_of_stay}, m_creation_date{time_t(nullptr)} {}

    // Creating a patient profile from a pre-existing patient profile
    PatientProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, std::string reason_of_admission, time_t time_of_admission, time_t expected_time_of_stay, time_t creation_date) : 
    m_profile_number{profile_number}, m_first_name{first_name}, m_last_name{last_name}, m_age{age}, m_reason_of_admission{reason_of_admission}, m_time_of_admission{time_of_admission}, m_expected_time_of_stay{expected_time_of_stay}, m_creation_date{creation_date} {}

    std::string getFirstName() const override { return m_first_name; }
    void setFirstName(std::string first_name) override { m_first_name = first_name; }

    std::string getLastName() const override { return m_last_name; }
    void setLastName(std::string last_name) override { m_last_name = last_name; }

    size_t getAge() const override { return m_age; }
    void setAge(size_t age) override { m_age = age; }

    time_t getCreationDate() const override { return m_creation_date; }

    size_t getProfileNumber() const override { return m_profile_number; }

    std::string getReasonOfAdmission() const { return m_reason_of_admission; }
    void setReasonOfAdmission(std::string reason_of_admission) { m_reason_of_admission = reason_of_admission; }
    
    time_t getTimeOfAdmission() const { return m_time_of_admission; }
    void setTimeOfAdmission(time_t time_of_admission) { m_time_of_admission = time_of_admission; }

    time_t getExpectedTimeOfStay() const { return m_expected_time_of_stay; }
    void setExpectedTimeOfStay(time_t expected_time_of_stay) { m_expected_time_of_stay = expected_time_of_stay; }

    std::string toString() const override { return std::format("{} {} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_reason_of_admission, m_time_of_admission, m_expected_time_of_stay); }
    std::string toFormattedString() const override
    {
        std::string profile_creation_date_string{ctime(&m_creation_date)};
        std::string time_of_admission_string{ctime(&m_time_of_admission)};
        std::string expected_time_of_stay_string{ctime(&m_expected_time_of_stay)};
        return std::format("Profile Number: {:<20d} | Last Name: {:<20s} | First Name: {:<20s} | Age: {:<3d}\nProfile Creation Date: {:<s}Reason of Admission: {:<s}\nTime of Admission: {:<s}Expected Time of Stay: {:<s}",
        m_profile_number, m_last_name, m_first_name, m_age, profile_creation_date_string, m_reason_of_admission, time_of_admission_string, expected_time_of_stay_string);
    }
};

class StaffProfile final : public Profile
{
    private:
    std::string m_first_name;                                // The first name of the person on profile.
    std::string m_last_name;                                 // The last name of the person on profile.
    size_t m_age;                                            // The age of the person on profile.
    time_t m_creation_date;                                  // The data the profile was created. This is assigned automatically, unless specified at the end of the constructor.
    size_t m_profile_number;                                 // The internal profile number. This is not created automatically, and must be manually assigned during construction.
    std::string m_occupation;                                // The occupation the staff member belongs to.
    size_t m_wage;                                           // The wage the staff member recieves. Uses a non-decimal number to avoid floating-point precision errors. Example: 2005 will be treated as $20.05/hr.
    std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> m_shifts;   // The shifts the staff member is working.
    Unit m_assigned_unit;                                    // The unit the staff member is assigned to. GENERAL for non-specific unit.
    std::vector<std::string> m_assigned_rooms;

    public:
    StaffProfile(size_t profile_number, std::string first_name = "<unknown>", std::string last_name = "<unknown>", size_t age = 0, std::string occupation = "<unknown>", size_t wage = 0, std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts = {}, Unit asssigned_unit = GENERAL, std::vector<std::string> assigned_rooms = {}) :
    m_profile_number{profile_number}, m_first_name{first_name}, m_last_name {last_name}, m_age{age}, m_occupation{occupation}, m_wage{wage}, m_shifts{shifts}, m_assigned_unit{asssigned_unit}, m_assigned_rooms{assigned_rooms}, m_creation_date{time_t(nullptr)} {}

    StaffProfile(size_t profile_number, std::string first_name, std::string last_name, size_t age, std::string occupation, size_t wage, time_t creation_date, std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts, Unit asssigned_unit, std::vector<std::string> assigned_rooms) :
    m_profile_number{profile_number}, m_first_name{first_name}, m_last_name {last_name}, m_age{age}, m_occupation{occupation}, m_wage{wage}, m_shifts{shifts}, m_assigned_unit{asssigned_unit}, m_assigned_rooms{assigned_rooms}, m_creation_date{creation_date} {}

    std::string getFirstName() const override { return m_first_name; }
    void setFirstName(std::string first_name) override { m_first_name = first_name; }

    std::string getLastName() const override { return m_last_name; }
    void setLastName(std::string last_name) override { m_last_name = last_name; }

    size_t getAge() const override { return m_age; }
    void setAge(size_t age) override { m_age = age; }

    time_t getCreationDate() const override { return m_creation_date; }

    size_t getProfileNumber() const override { return m_profile_number; }

    std::string getOccupation() const { return m_occupation; }
    void setOccupation(std::string occupation) { m_occupation = occupation; }

    size_t getWage() const { return m_wage; }
    void setWage(size_t wage) { m_wage = wage; }

    std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> getShifts() const { return m_shifts; }
    void setShifts(std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts) { m_shifts = shifts; }

    Unit getAssignedUnit() const { return m_assigned_unit; }
    void setAssignedUnit(Unit assigned_unit) { m_assigned_unit = assigned_unit; }

    std::vector<std::string> getAssignedRooms() const { return m_assigned_rooms; }
    void setAssignedRooms(std::vector<std::string> assigned_rooms) { m_assigned_rooms = assigned_rooms; }

    std::string toString() const override { return std::format("{} {} {} {} {} {} {}", m_profile_number, m_first_name, m_last_name, m_age, m_creation_date, m_occupation, m_wage); }
    std::string toFormattedString() const override
    {
        std::string profile_creation_date_string{ctime(&m_creation_date)};
        return std::format("Profile Number: {:<20d} | Last Name: {:<20s} | First Name: {:<20s} | Age: {:<3d}\nProfile Creation Date: {:<s}Occupation: {:<s}\nWage: {:<d}\n",
        m_profile_number, m_last_name, m_first_name, m_age, profile_creation_date_string, m_occupation, m_wage);
    }
};
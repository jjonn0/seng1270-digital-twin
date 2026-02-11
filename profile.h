#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <format>

class Profile
{
    protected:
    std::string m_name; // The name of the person on profile
    size_t m_age; // The age of the person on profile
    time_t m_creation_date; // [Cannot be changed] The data the profile was created. This is assigned automatically, unless specified at the end of the constructor.
    size_t m_profile_number; // [Cannot be changed] The internal profile number. This is not created automatically, and must be manually assigned during construction.
    
    void constructionMessage()
    {
        std::cout << std::format("New profile #{} was created.\n", m_profile_number);
    }
    void destructionMessage()
    {
        std::cout << std::format("Profile #{} was destroyed.\n", m_profile_number);
    }

    public:
    /**
     * @brief                   Default constructor for making a brand new profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param name              The name of the person on profile.
     * @param age               The age of the person on profile.
     */
    Profile(size_t profile_number, std::string name = "<unknown>", size_t age = 0) : m_profile_number{profile_number}, m_name{name}, m_age{age}
    {
        constructionMessage();
        time(&m_creation_date);
    }
    /**
     * @brief                   Default constructor for making a brand new profile.
     * @param profile_number    A number used to reference the profile. This must be created manually, cannot conflict with any other number, and cannot be changed after creation.
     * @param name              The name of the person on profile.
     * @param age               The age of the person on profile.
     * @param m_creation_date   The date that the original profile was created on.
     */
    Profile(size_t profile_number, std::string name, size_t age, time_t creation_date) : m_profile_number{profile_number}, m_name{name}, m_age{age}, m_creation_date{creation_date} { constructionMessage(); }
    ~Profile() { destructionMessage(); }

    std::string getName() { return m_name; }
    size_t getAge() { return m_age; }
    time_t getCreationDate() { return m_creation_date; }
    size_t getProfileNumber() { return m_profile_number; }

    void setName(std::string name) { m_name = name; }
    void setAge(size_t age) { m_age = age; }
};
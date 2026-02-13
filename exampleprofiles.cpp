#include <iostream>
#include <chrono>
#include <typeinfo>
#include <format>
#include "profile.h"
#include "patientprofile.h"
#include "staffprofile.h"
using namespace std;

time_t getTimestamp(size_t year, size_t month, size_t day, size_t hour, size_t minute, size_t second)
{
    struct tm datetime;
    datetime.tm_year = year - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = day;
    datetime.tm_hour = hour;
    datetime.tm_min = minute;
    datetime.tm_sec = second;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

int main()
{
    Profile profile_1{0}; // Create a non-descript profile of number 0.
    cout << format("Profile #{}, First Name {}, Age {}, Creation time {}\n", profile_1.getProfileNumber(), profile_1.getFirstName(), profile_1.getAge(), profile_1.getCreationDate()); // Note that getCreationDate returns time in seconds from Jan 1, 1970.

    time_t timestamp{profile_1.getCreationDate()}; // Create a timestamp variable for referencing.
    cout << format("Creation Date in timestamp format: {}\n", ctime(&timestamp));

    Profile profile_2{1, "John", "Doe", 24}; // Create a profile of number 1 with name John Doe and age 24.
    cout << format("Profile #{}, First Name {}, Age {}, Creation Date {}\n", profile_2.getProfileNumber(), profile_2.getFirstName(), profile_2.getAge(), profile_2.getCreationDate());

    PatientProfile profile_3{2, "Jane", "Doe", 26};
    profile_3.setReasonOfAdmission("Ate too much Little Caesers crazy bread"); // Set reason of patient admission.
    timestamp = getTimestamp(2026, 1, 14, 13, 53, 12);
    profile_3.setTimeOfAdmission(timestamp);
    cout << format("Reason of admission: {}\nAdmission time: {}\nDate of admission {}\n", profile_3.getReasonOfAdmission(), profile_3.getTimeOfAdmission(), ctime(&timestamp));

    return 0;
}
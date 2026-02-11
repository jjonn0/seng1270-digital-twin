#include <iostream>
#include <chrono>
#include <typeinfo>
#include "profile.h"
#include "patientprofile.h"
using namespace std;

int main()
{
    {
        Profile profile1{0};
        cin.ignore();
        PatientProfile patient1{1, "Jim", 34};

        time_t timestamp = profile1.getCreationDate();
        cout << "Profile 1 creation: " << ctime(&timestamp);
        timestamp = patient1.getCreationDate();
        cout << "Patient 1 creation: " << ctime(&timestamp);

        struct tm datetime;
        datetime.tm_year = 2026 - 1900; // Set year to 2026
        datetime.tm_mon = 10 - 1; // Set month to October
        datetime.tm_mday = 31; // Set day to 31
        datetime.tm_hour = 0;
        datetime.tm_min = 0;
        datetime.tm_sec = 0;
        datetime.tm_isdst = -1;
        timestamp = mktime(&datetime);
        cout << "Patient 1 expected time of stay: " << ctime(&timestamp) << endl;
    }

    return 0;
}
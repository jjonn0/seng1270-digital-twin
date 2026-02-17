#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include "profiles.h"
#include "simulationentity.h"
using namespace std;

vector<SimulationEntity> entity_list;

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
    entity_list = {SimulationEntity(PatientProfile{1, "Jim", "Davis", 55, "Ate too much lasanga.", getTimestamp(2025, 5, 13, 12, 37, 0), getTimestamp(2025, 5, 15, 12, 0, 0)}, StaffProfile{0}),
                    SimulationEntity(PatientProfile{2, "Jerry", "Slime", 27, "Woke up on the wrong side of the bed.", getTimestamp(1990, 8, 29, 3, 47, 0), getTimestamp(2050, 8, 29, 12, 0, 0)}, StaffProfile{0}),
                    SimulationEntity(PatientProfile{0}, StaffProfile{1, "Kaylee", "Carrigan", 28, "Doctor", 7200}),
                    SimulationEntity(PatientProfile{0}, StaffProfile{2, "Henry", "Verdana", 31, "Nurse", 3400})};
    
    printCurrentEntities(entity_list);

    return 0;
}
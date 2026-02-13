#include <iostream>
#include <array>
#include <format>
#include "rooms.h"
#include "patientprofile.h"
#include "staffprofile.h"
using namespace std;

array<PatientProfile, 4> patients{
    PatientProfile(0, "Jim", "Baker", 25),
    PatientProfile(1, "Dianne", "White", 43),
    PatientProfile(2, "Chris", "Rock", 18),
    PatientProfile(3, "Sergio", "Yuletide", 95)};

void printCurrentEntities(string list_name, auto& entity_list);

int main()
{
    printCurrentEntities("patients", patients);

    return 0;
}

void printCurrentEntities(string list_name, auto& entity_list)
{
    cout << format("Current {} in simulation: \n", list_name);
    for(auto& e : entity_list)
    {
        cout << e.toString() << endl;
    }
}
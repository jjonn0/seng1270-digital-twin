#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include <string>
#include <sstream>
#include <regex>
#include "profiles.h"
#include "simulationentity.h"
using namespace std;

vector<PatientProfile> patients;
vector<StaffProfile> staff;

time_t getTimestamp(size_t year, size_t month, size_t day, size_t hour, size_t minute, size_t second);
void displayOptionsMenu(time_t current_time);
void displayModifyProfilesMenu();

int main()
{
    patients = {PatientProfile{1, "Jim", "Davis", 55, "Ate too much lasanga.", getTimestamp(2025, 5, 13, 12, 37, 0), getTimestamp(2025, 5, 15, 12, 0, 0)},
                PatientProfile{2, "Jerry", "Slime", 27, "Woke up on the wrong side of the bed.", getTimestamp(1990, 8, 29, 3, 47, 0), getTimestamp(2050, 8, 29, 12, 0, 0)},
                PatientProfile{2, "Laurel", "Yanny", 42, "Every word sounds the same to them.", getTimestamp(2020, 7, 2, 3, 23, 0), getTimestamp(2026, 2, 28, 12, 0, 0)}};
    staff = {StaffProfile{1, "Kaylee", "Carrigan", 28, "Doctor", 7200},
             StaffProfile{2, "Henry", "Verdana", 31, "Nurse", 3400}};
    
    // Main loop
    string input{""};
    while(true)
    {
        time_t current_time;
        time(&current_time);
        displayOptionsMenu(current_time);
        if(cin >> input)
        {
            try
            {
                int value{stoi(input)};
                switch(value)
                {
                    case 1:
                        displayModifyProfilesMenu();
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        throw 0;
                }
            }
            catch(...)
            {
                cout << "\nAn invalid input was given. Please try again.\n\n";
            }
        }
        else { break; }
    }
    
    return 0;
}

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

void displayOptionsMenu(time_t current_time)
{
    string modified_date_text{ctime(&current_time)};
    modified_date_text.erase(modified_date_text.size() - 1);
    cout << format("\n{:<27s}{:>30s}\n\n{:<s}\n{:<s}\n{:<s}\n{:<s}\n\n>",
        "VitalVitals Simulation Menu", modified_date_text, "1 - Modify Profiles", "2 - Status Update", "3 - Modify Current Time", "CTRL+Z - Exit Program");
}

void displayModifyProfilesMenu()
{
    string input{""};

    auto displayMenu([](auto profiles){
        const size_t entries_per_page{2};
        const size_t page_count{profiles.size() / entries_per_page};
        size_t current_page{0};
        string input{""};

        while(true)
        {
            cout << format("\n{:<6s} | {:<20s} | {:<20s}\n", "Index", "Last Name", "First Name");
            size_t entry_count_to_back{profiles.size() - (current_page * entries_per_page)};
            size_t entries_on_page{(entries_per_page < entry_count_to_back) ? entries_per_page : entry_count_to_back}; // Find out how many entities are on the current page

            for(size_t index{current_page * entries_per_page}; index < entries_on_page + current_page * entries_per_page; index++)
            {
                cout << format("{:<6d} | {:<20s} | {:<20s}\n", profiles[index].getProfileNumber(), profiles[index].getLastName(), profiles[index].getFirstName());
            }
            cout << format("Page ({}/{})\n", current_page, page_count);
            cout << format("{}\n{}\n{}\n\n>", "p# - Switch to Page", "Type the Shown Index to View/Edit Profile", "CTRL+Z - Go Back");
            
            if(cin >> input)
            {
                regex page_pattern(R"(^(p|P)\d+$)"); // Matches with any string starting with a p or P and ending with any number
                try
                {
                    if(regex_match(input, page_pattern))
                    {
                        regex letter_stripper(R"(([a-z]|[A-Z]))"); // Matches with any letter character
                        input = regex_replace(input, letter_stripper, "");
                        current_page = stoi(input);
                        continue;
                    }
                }
                catch(...)
                {
                     cout << "\nAn invalid input was given. Please try again.";
                }
            }
            else { break; }
        }
        
        cin.clear();
    });
    
    while(true)
    {
        cout << "VitalVitals Profile Modification Menu\n\n";
        cout << format("{}\n{}\n{}\n\n>", "1 - Display Patient Profiles", "2 - Display Staff Profiles", "CTRL+Z - Go Back");

        if(cin >> input)
        {
            try
            {
                if(input == "1")
                {
                    displayMenu(patients);
                }
                else if(input == "2")
                {
                    displayMenu(staff);
                }
                else { throw; }
            }
            catch(...)
            {  
                cout << "\nAn invalid input was given. Please try again.";
            }
        }
        else { break; }
    }

    cin.clear();
}
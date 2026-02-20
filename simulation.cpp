#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include <string>
#include <sstream>
#include <regex>
#include <cmath>
#include "profiles.h"
#include "simulationentity.h"
using namespace std;

vector<PatientProfile> patients;
vector<StaffProfile> staff;

time_t getTimestamp(size_t year, size_t month, size_t day, size_t hour, size_t minute, size_t second);
void displayErrorMessage(string message);
void displayOptionsMenu(time_t current_time);
void displayModifyProfilesMenu();
void displayProfileModifyTable(auto& profiles);
void modifyProfileMenu(auto& profile);

int main()
{
    patients = {PatientProfile{1, "Jim", "Davis", 55, "Ate too much lasanga.", getTimestamp(2025, 5, 13, 12, 37, 0), getTimestamp(2025, 5, 15, 12, 0, 0)},
                PatientProfile{2, "Jerry", "Slime", 27, "Woke up on the wrong side of the bed.", getTimestamp(1990, 8, 29, 3, 47, 0), getTimestamp(2050, 8, 29, 12, 0, 0)},
                PatientProfile{3, "Laurel", "Yanny", 42, "Every word sounds the same to them.", getTimestamp(2020, 7, 2, 3, 23, 0), getTimestamp(2026, 2, 28, 12, 0, 0)}};
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
            catch(...) { displayErrorMessage("\nAn invalid input was given. Enter anything to continue.\n"); }
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

void displayErrorMessage(string message)
{
    cerr << endl << message << endl;
    cin.ignore();
    cin.get();
}

/**
 * @brief   The main menu that allows the user to modify profiles, generate a status update, or modify the current time to see future states.
 */
void displayOptionsMenu(time_t current_time)
{
    string modified_date_text{ctime(&current_time)};
    modified_date_text.erase(modified_date_text.size() - 1);
    cout << format("\n{:<27s}{:>30s}\n\n{:<s}\n{:<s}\n{:<s}\n{:<s}\n\n>",
        "VitalVitals Simulation Menu", modified_date_text, "1 - Modify Profiles", "2 - Status Update", "3 - Modify Current Time", "CTRL+Z - Exit Program");
}

/**
 * @brief   A menu that allows the user to access either all of PatientProfile objects or StaffProfile objects.
 */
void displayModifyProfilesMenu()
{
    string input{""};
    
    while(true)
    {
        cout << "\nProfile Modification Menu\n\n";
        cout << format("{}\n{}\n{}\n\n>", "1 - Display Patient Profiles", "2 - Display Staff Profiles", "CTRL+Z - Go Back");

        if(cin >> input)
        {
            try
            {
                if(input == "1")
                {
                    displayProfileModifyTable(patients);
                }
                else if(input == "2")
                {
                    displayProfileModifyTable(staff);
                }
                else { throw; }
            }
            catch(...) { displayErrorMessage("\nAn invalid input was given. Enter anything to continue.\n"); }
        }
        else { break; }
    }

    cin.clear();
}

/**
 * @brief           A menu that displays the profiles in a paged table format. Used to access profiles for viewing/modifying.
 * @param profiles  A vector of either PatientProfile or StaffProfile class-type.
 */
void displayProfileModifyTable(auto& profiles)
{
    const size_t entries_per_page{2};
    const size_t page_count{size_t(floor(profiles.size() / entries_per_page))};
    size_t current_page{0};
     string input{""};

    while(true)
    {
        cout << format("\n{:<6s} | {:<20s} | {:<20s} | {:<20s}\n", "Index", "Profile Number", "Last Name", "First Name");
        size_t entry_count_to_back{profiles.size() - (current_page * entries_per_page)};
        size_t entries_on_page{(entries_per_page < entry_count_to_back) ? entries_per_page : entry_count_to_back}; // Find out how many entities are on the current page.

        for(size_t index{current_page * entries_per_page}; index < entries_on_page + current_page * entries_per_page; index++)
        {
            cout << format("{:<6d} | {:<20d} | {:<20s} | {:<20s}\n", index, profiles[index].getProfileNumber(), profiles[index].getLastName(), profiles[index].getFirstName());
        }
        cout << format("Page ({}/{})\n", current_page, page_count);
        cout << format("{}\n{}\n{}\n\n>", "p# - Switch to Page", "Type the Shown Index to View/Edit Profile", "CTRL+Z - Go Back");
        
        if(cin >> input)
        {
            regex page_pattern(R"(^(p|P)\d+$)"); // Matches with any string starting with a p or P and ending with any number.
            regex index_pattern(R"(^\d+$)");
            try
            {
                if(regex_match(input, page_pattern))
                {
                    regex letter_stripper(R"(([a-z]|[A-Z]))"); // Matches with any letter character
                    input = regex_replace(input, letter_stripper, "");
                    current_page = min(stoi(input), int(page_count));
                    continue;
                }
                else if(regex_match(input, index_pattern))
                {
                    int index_numerical{stoi(input)};
                    if(index_numerical >= profiles.size()) { displayErrorMessage("\nThe index entered exceeded the maximum value. Enter anything to continue.\n"); }
                    modifyProfileMenu(profiles[index_numerical]);
                }
                else { throw 0; }
            }
            catch(...) { displayErrorMessage("\nAn invalid input was given. Enter anything to continue.\n"); }
        }
        else { break; }
    }
        
    cin.clear();
}

/**
 * @brief   A menu to allow the user to edit either a PatientProfile or StaffProfile.
 */
void modifyProfileMenu(auto& profile)
{
    string patient_profile_id{typeid(PatientProfile).name()};
    string staff_profile_id{typeid(StaffProfile).name()};
    string input_profile_id{typeid(profile).name()};
    string input{""};
    
    if(patient_profile_id == input_profile_id || staff_profile_id == input_profile_id)
    {
        auto readNewValue([](){
            cout << "\nWhat should the new value be?: ";
            string input{""};
            cin.ignore();
            getline(cin, input);
            return input;
        });
        auto readNewDate([](){
            cout << "Enter the year, month, day, hours, minutes, and seconds seperated by spaces and all in numerical values: ";
            cin.ignore();
            size_t year{0}, month{0}, day{0}, hour{0}, minute{0}, second{0};
            cin >> year >> month >> day >> hour >> minute >> second;
            return getTimestamp(year, month, day, hour, minute, second);
        });

        while(true)
        {
            cout << endl << profile.toFormattedString();

            try
            {
                if(patient_profile_id == input_profile_id)
                {
                    cout << format("What would you like to edit?\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n\n>", "1 - Last Name", "2 - First Name", "3 - Age", "4 - Reason of Admission", "5 - Time of Admission", "6 - Expected Time of Stay", "CTRL+Z - Go Back");
                    if(cin >> input)
                    {
                        switch(stoi(input))
                        {
                            case 1:
                                profile.setLastName(readNewValue());
                                break;
                            case 2:
                                profile.setFirstName(readNewValue());
                                break;
                            case 3:
                                profile.setAge(stoi(readNewValue()));
                                break;
                            case 4:
                                profile.setReasonOfAdmission(readNewValue());
                                break;
                            case 5:
                                profile.setTimeOfAdmission(readNewDate());
                                break;
                            case 6:
                                profile.setExpectedTimeOfStay(readNewDate());
                                break;
                            default:
                                throw 0;
                        }
                    }
                    else { break; }
                }
                else if(staff_profile_id == input_profile_id)
                {
                    cout << format("What would you like to edit?\n{}\n{}\n{}\n{}\n{}\n{}\n\n>", "1 - Last Name", "2 - First Name", "3 - Age", "4 - Role", "5 - Wage", "CTRL+Z - Go Back");
                    if(cin >> input)
                    {
                        switch(stoi(input))
                        {
                            case 1:
                                profile.setLastName(readNewValue());
                                break;
                            case 2:
                                profile.setFirstName(readNewValue());
                                break;
                            case 3:
                                profile.setAge(stoi(readNewValue()));
                                break;
                            case 4:
                                profile.setRole(readNewValue());
                                break;
                            case 5:
                                profile.setWage(stoi(readNewValue()));
                                break;
                            default:
                                throw 0;
                        }
                    }
                    else { break; }
                }
            }
            catch(...) { displayErrorMessage("An error occured. Please enter anything to continue."); }
        }
    }
    else
    {
        displayErrorMessage("\nAn invalid profile was given. Enter anything to continue.\n");
    }
    cin.clear();
}
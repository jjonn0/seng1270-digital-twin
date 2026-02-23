#include <iostream>
#include <vector>
#include <format>
#include <typeinfo>
#include <string>
#include <sstream>
#include <regex>
#include <cmath>
#include <algorithm>
#include "profiles.h"
#include "simulationentity.h"
using namespace std;

vector<PatientProfile> patients;
vector<StaffProfile> staff;

time_t getTimestamp(size_t year, size_t month, size_t day, size_t hour, size_t minute, size_t second);
time_t readNewDate();
void displayErrorMessage(string message);
void displayOptionsMenu(time_t current_time);
void displayModifyProfilesMenu();
void displayProfileModifyTable(auto& profiles);
void modifyProfileMenu(auto& profile);
PatientProfile createNewProfile(vector<PatientProfile> patient_profiles);
StaffProfile createNewProfile(vector<StaffProfile> staff_profiles);
void displayStatusReportMenu(time_t old_timestamp, time_t new_timestamp);
string generateStatusReport(time_t old_timestamp, time_t new_timestamp, auto profiles);

int main()
{
    patients = {PatientProfile{1, "Jim", "Davis", 55, "Ate too much lasanga.", getTimestamp(2025, 5, 13, 12, 37, 0), getTimestamp(2025, 5, 15, 12, 0, 0)},
                PatientProfile{2, "Jerry", "Slime", 27, "Woke up on the wrong side of the bed.", getTimestamp(1990, 8, 29, 3, 47, 0), getTimestamp(2050, 8, 29, 12, 0, 0)},
                PatientProfile{3, "Laurel", "Yanny", 42, "Every word sounds the same to them.", getTimestamp(2020, 7, 2, 3, 23, 0), getTimestamp(2026, 2, 28, 12, 0, 0)}};
    staff = {StaffProfile{1, "Kaylee", "Carrigan", 28, "Doctor", 7200},
             StaffProfile{2, "Henry", "Verdana", 31, "Nurse", 3400}};
    
    time_t current_time, previous_time;
    time(&current_time);
    previous_time = current_time;
    // Main loop
    string input{""};
    while(true)
    {
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
                        displayStatusReportMenu(previous_time, current_time);
                        break;
                    case 3:
                        previous_time = current_time;
                        current_time = readNewDate();
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

time_t readNewDate()
{
    cout << "Enter the year, month, day, hours, minutes, and seconds seperated by spaces and all in numerical values: ";
    cin.ignore();
    size_t year{0}, month{0}, day{0}, hour{0}, minute{0}, second{0};
    cin >> year >> month >> day >> hour >> minute >> second;
    return getTimestamp(year, month, day, hour, minute, second);
};

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
    const size_t entries_per_page{10};
    string input{""};
    size_t current_page{0};

    while(true)
    {
        size_t page_count{size_t(floor(profiles.size() / entries_per_page))};
        size_t entry_count_to_back{profiles.size() - (current_page * entries_per_page)};
        size_t entries_on_page{(entries_per_page < entry_count_to_back) ? entries_per_page : entry_count_to_back}; // Find out how many entities are on the current page.
        
        cout << format("\n{:<6s} | {:<20s} | {:<20s} | {:<20s}\n", "Index", "Profile Number", "Last Name", "First Name");
        
        for(size_t index{current_page * entries_per_page}; index < entries_on_page + current_page * entries_per_page; index++)
        {
            cout << format("{:<6d} | {:<20d} | {:<20s} | {:<20s}\n", index, profiles[index].getProfileNumber(), profiles[index].getLastName(), profiles[index].getFirstName());
        }
        cout << format("Page ({}/{})\n", current_page, page_count);
        cout << format("{}\n{}\n{}\n{}\n{}\n\n>", "p# - Switch to Page", "# - Index to View/Edit Profile", "a - Add New Profile", "r# - Remove Profile", "CTRL+Z - Go Back");
        
        if(cin >> input)
        {
            regex page_pattern(R"(^(p|P)\d+$)"); // Matches with any string starting with a p or P and ending with any number.
            regex index_pattern(R"(^\d+$)");
            regex removal_pattern(R"(^(r|R)\d+$)");
            regex letter_stripper(R"(([a-z]|[A-Z]))"); // Matches with any letter character
            try
            {
                if(regex_match(input, page_pattern))
                {
                    input = regex_replace(input, letter_stripper, "");
                    current_page = min(stoi(input), int(page_count));
                    continue;
                }
                else if(regex_match(input, index_pattern))
                {
                    int index_numerical{stoi(input)};
                    if(index_numerical >= profiles.size()) { displayErrorMessage("\nThe index entered exceeded the maximum value. Enter anything to continue.\n"); }
                    displayModifyProfileMenu(profiles[index_numerical]);
                }
                else if(input == "a")
                {
                    auto new_profile{createNewProfile(profiles)};
                    profiles.push_back(new_profile);
                    displayModifyProfileMenu(profiles[profiles.size() - 1]);
                }
                else if(regex_match(input, removal_pattern))
                {
                    input = regex_replace(input, letter_stripper, "");
                    int index_numerical{stoi(input)};
                    if(index_numerical >= profiles.size()) { displayErrorMessage("\nThe index entered exceeded the maximum value. Enter anything to continue.\n"); }
                    for(index_numerical; index_numerical < profiles.size() - 1; index_numerical++) { profiles[index_numerical] = profiles[index_numerical + 1]; }
                    profiles.pop_back();
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
void displayModifyProfileMenu(auto& profile)
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

/**
 * @brief   Creates a new profile, appends it to the given vector, and then returns the new profile.
 */
PatientProfile createNewProfile(vector<PatientProfile> patient_profiles)
{
    size_t next_highest_number{0};
    vector<size_t> active_profile_numbers;

    for(PatientProfile e : patient_profiles)
    {
        active_profile_numbers.push_back(e.getProfileNumber());
    }

    next_highest_number = *(max_element(active_profile_numbers.begin(), active_profile_numbers.end()));

    PatientProfile new_profile = PatientProfile{next_highest_number};
    return new_profile;
}
/**
 * @brief   Creates a new profile, appends it to the given vector, and then returns the new profile.
 */
StaffProfile createNewProfile(vector<StaffProfile> staff_profiles)
{
    size_t next_highest_number{0};
    vector<size_t> active_profile_numbers;

    for(StaffProfile e : staff_profiles)
    {
        active_profile_numbers.push_back(e.getProfileNumber());
    }

    next_highest_number = *(max_element(active_profile_numbers.begin(), active_profile_numbers.end())) + 1;

    StaffProfile new_profile = StaffProfile{next_highest_number};
    return new_profile;
}

void displayStatusReportMenu(time_t old_timestamp, time_t new_timestamp)
{
    string input{""};
    while(true)
    {
        cout << "\nStatus Report Menu\n\n";
        cout << format("{}\n{}\n{}\n\n>", "1 - Patient Status Report", "2 - Staff Status Report", "CTRL+Z - Go Back");
        if(cin >> input)
        {
            try
            {
                switch(stoi(input))
                {
                    case 1:
                        cout << generateStatusReport(old_timestamp, new_timestamp, patients);
                        break;
                    case 2:
                        cout << generateStatusReport(old_timestamp, new_timestamp, staff);
                        break;
                    default:
                        throw 0;
                }
            }
            catch(...)
            {
                displayErrorMessage("An invalid input was given. Enter anything to continue.");
            }
        }
        else { break; }
    }
    
    cin.clear();
}

string generateStatusReport(time_t old_timestamp, time_t new_timestamp, auto profiles)
{
    string output{""};
    bool is_empty{true};

    // Takes in two points in time and the time the event is to occur, and returns true or false. Requires disambiguation.
    auto hasOccured([](time_t old_timestamp, time_t new_timestamp, time_t occurence_time){
        bool after_old_timestamp{false}, after_new_timestamp{false};
        after_old_timestamp = (occurence_time > old_timestamp ? true : false);
        after_new_timestamp = (occurence_time > new_timestamp ? true : false);
        if(after_old_timestamp != after_new_timestamp) { return true; }
        return false;
    });

    vector<PatientProfile> patient_reference_vector{};
    vector<StaffProfile> staff_reference_vector{};

    string patient_vector_id{typeid(patient_reference_vector).name()};
    string staff_vector_id{typeid(staff_reference_vector).name()};
    string profiles_id{typeid(profiles).name()};

    // Patient status report
    if(profiles_id == patient_vector_id)
    {
        output += "PATIENT STATUS REPORT\n";
        for(auto e : profiles)
        {
            time_t discharge_timestamp{e.getExpectedTimeOfStay()};

            if(hasOccured(old_timestamp, new_timestamp, discharge_timestamp))
            {
                if(old_timestamp < new_timestamp) { output += format("{:<20d} | {:<20s} | {:<20s} | Discharged.\n", e.getProfileNumber(), e.getLastName(), e.getFirstName()); }
                if(old_timestamp > new_timestamp) { output += format("{:<20d} | {:<20s} | {:<20s} | Un-discharged.\n", e.getProfileNumber(), e.getLastName(), e.getFirstName()); }
                is_empty = false;
            }
        }
    }

    // Staff status report
    if(profiles_id == staff_vector_id)
    {
        
    }

    return (is_empty ? "Report empty." : output);
}
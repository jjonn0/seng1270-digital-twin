/** @warning I apologize for what you are about to read */
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <fstream>
#include "rapidcsv.h"
#include "profiles.h"

struct columnNames {
    std::string c_profile_number{"Profile number"};
    std::string c_first_name{"First name"};
    std::string c_last_name{"Last name"};
    std::string c_dob{"Date of birth"};
    std::string c_reason_of_admission{"Reason of admission"};
    std::string c_time_of_admission{"Time of admission"};
    std::string c_expected_time_of_stay{"Expected time of stay"};
    std::string c_creation_date{"Creation date"};
    std::string c_admitted_unit{"Admitted unit"};
    std::string c_occupation{"Occupation"};
    std::string c_wage{"Wage"};
    std::string c_shifts{"Shifts"};
    std::string c_assigned_unit{"Assigned unit"};
    std::string c_assigned_rooms{"Assigned rooms"};
} columns; 

class FileHandler
{
    private:
    
    void pushError(std::string error_message)
    {
        std::cerr << "\033[91m" + std::string("FileHandler encountered an error: ") + error_message + "\033[97m" << std::endl;
    }
    
    public:
    /// @brief loads the data from the patient data file. Is static and can be used anywhere, no need to make a filehandler object
    /// @param filename the name of the file for patient data
    /// @return an std::vector of patient profiles containing every profile in the file
    static std::vector<PatientProfile> loadPatientData(const std::string filename) { // reads the patient file, and loads all of the data into memory
        rapidcsv::Document file(filename);
        std::vector<PatientProfile> profiles;

        for (size_t i{0}; i < file.GetRowCount(); i++) {
            size_t profile_number = file.GetCell<size_t>(columns.c_profile_number, i);
            std::string first_name = file.GetCell<std::string>(columns.c_first_name, i);
            std::string last_name = file.GetCell<std::string>(columns.c_last_name, i);
            time_t dob = file.GetCell<time_t>(columns.c_dob, i);
            std::string reason_of_admission = file.GetCell<std::string>(columns.c_reason_of_admission, i);
            time_t time_of_admission = file.GetCell<time_t>(columns.c_time_of_admission, i);
            time_t expected_time_of_stay = file.GetCell<time_t>(columns.c_expected_time_of_stay, i);
            time_t creation_date = file.GetCell<time_t>(columns.c_creation_date, i);
            Unit assigned_unit = static_cast<Unit>(file.GetCell<int>(columns.c_admitted_unit, i));
            profiles.emplace_back(profile_number, first_name, last_name, dob, reason_of_admission, time_of_admission, expected_time_of_stay, creation_date, assigned_unit); // creates a new object for each row, and sticks it in a vector
        }
        return profiles;
    }

    // The time block array is saved by saving the time_block_start and end with delimeters: 0\0|1\1|2\2|3\3...
    static std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> parseIntoTimeBlocks(std::string info) {
        std::stringstream wholestring(info), single_section; //single selection is one set of start and end values, separated by a backslash
        std::string processed_data{""}, start_string, end_string; // processed_data is added into the single selection stream
        time_t start, end;
        std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> result {};

        for (size_t array_index{0}; array_index < MAXIMUM_STORED_SHIFTS; array_index++) {
            single_section.clear();
            single_section.str("");
            if (!getline(wholestring, processed_data, '|')) break;
            single_section << processed_data;
            if (!getline(single_section, start_string, '\\')) break;
            if (!getline(single_section, end_string, '\\')) break;
            start = static_cast<time_t>(stoi(start_string));
            end = static_cast<time_t>(stoi(end_string));
            result[array_index].time_block_start = start;
            result[array_index].time_block_end = end; 
        }
        return result;
    }

    static std::vector<std::string> parseIntoRooms(std::string info) {
        std::stringstream roomStream(info);
        std::vector<std::string> result;
        std::string room;
        while (getline(roomStream, room, '|')) {
            result.push_back(room);
        }
        return result;
    }
    

    /// @brief loads the data from the staff data file. Is static and can be used anywhere, no need to make a filehandler object
    /// @param filename the name of the staff data file
    /// @return an std::vector of staff profiles containing every profile in the file
    static std::vector<StaffProfile> loadStaffData(const std::string filename) { // reads the staff file, and loads all of the data into memory
        rapidcsv::Document file(filename);
        std::vector<StaffProfile> profiles;
        for (size_t i{0}; i < file.GetRowCount(); i++) {
            size_t profile_number = file.GetCell<size_t>(columns.c_profile_number, i);
            std::string first_name = file.GetCell<std::string>(columns.c_first_name, i);
            std::string last_name = file.GetCell<std::string>(columns.c_last_name, i);
            time_t dob = file.GetCell<time_t>(columns.c_dob, i);
            std::string occupation = file.GetCell<std::string>(columns.c_occupation, i);
            size_t wage = file.GetCell<size_t>(columns.c_wage, i);
            std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts = parseIntoTimeBlocks(file.GetCell<std::string>(columns.c_shifts, i));
            Unit assigned_unit = static_cast<Unit>(file.GetCell<int>(columns.c_assigned_unit, i));
            std::vector<std::string> assigned_rooms = parseIntoRooms(file.GetCell<std::string>(columns.c_assigned_rooms, i));
            time_t creation_date = file.GetCell<time_t>(columns.c_creation_date, i);
            profiles.emplace_back(profile_number, first_name, last_name, dob, occupation, wage, creation_date, shifts, assigned_unit, assigned_rooms);
        }
        return profiles;
    }
    
    /// @brief dont use this function, use savePatientData for a single patient, and saveAllPatientData for a vector of patient data 
    static void writePatientData(PatientProfile patient, std::string filename, size_t row) {
        rapidcsv::Document file(filename);
        file.SetCell(0, row, patient.getProfileNumber());
        file.SetCell(1, row, patient.getFirstName());
        file.SetCell(2, row, patient.getLastName());
        file.SetCell(3, row, patient.getDOB());
        file.SetCell(4, row, patient.getReasonOfAdmission());
        file.SetCell(5, row, patient.getTimeOfAdmission());
        file.SetCell(6, row, patient.getExpectedTimeOfStay());
        file.SetCell(7, row, patient.getCreationDate());
        file.SetCell(8, row, patient.getAdmittedUnitInt());
        file.Save(filename);
    }

    static void writeStaffData(StaffProfile staff, std::string filename, size_t row) {
        rapidcsv::Document file(filename);
        file.SetCell(0, row, staff.getProfileNumber());
        file.SetCell(1, row, staff.getFirstName());
        file.SetCell(2, row, staff.getLastName());
        file.SetCell(3, row, staff.getDOB());
        file.SetCell(4, row, staff.getOccupation());
        file.SetCell(5, row, staff.getWage());
        file.SetCell(6, row, staff.getShiftExportString());
        file.SetCell(7, row, static_cast<int>(staff.getAssignedUnit()));
        file.SetCell(8, row, staff.getRoomString());
        file.SetCell(9, row, staff.getCreationDate());
        file.Save(filename);
    }

    /// @brief Saves the data for a single patient. Uses the profile number to reference
    static void savePatientData(PatientProfile patient, std::string filename) {
        rapidcsv::Document file(filename);
        for(size_t row{0}; row < file.GetRowCount() - 1; row++) {
            if (file.GetCell<size_t>(columns.c_profile_number, row) == patient.getProfileNumber()) {
                writePatientData(patient, filename, row);
                return;
            }
        }
        writePatientData(patient, filename, (file.GetRowCount())); // Saves to a new row
    }

    /// @brief Saves the data for a single staff member. Uses the profile number to reference
    static void saveStaffData(StaffProfile staff, std::string filename) {
        rapidcsv::Document file(filename);
        for(size_t row{0}; row < file.GetRowCount(); row++) {
            if (file.GetCell<size_t>(columns.c_profile_number, row) == staff.getProfileNumber()) {
                writeStaffData(staff, filename, row);
                return;
            }
        }
        writeStaffData(staff, filename, (file.GetRowCount())); // Saves to a new row
    }
};
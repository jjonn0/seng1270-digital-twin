/** @warning I apologize for what you are about to read */

/// @date   March 30, 2026
/// @brief  Updated functions for saving/loading that incorporate EncDec for encryption and decryption. If you wish to note have encryption/decryption take place, enter a blank password.

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <fstream>
#include "rapidcsv.h"
#include "profiles.h"
#include "encdec.h"

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
    
    static void pushError(std::string error_message)
    {
        std::cerr << "\033[91m" + std::string("FileHandler encountered an error: ") + error_message + "\033[97m" << std::endl;
    }
    
    public:
    /// @brief loads the data from the patient data file. Is static and can be used anywhere, no need to make a filehandler object
    /// @param filename the name of the file for patient data
    /// @return an std::vector of patient profiles containing every profile in the file
    static std::vector<PatientProfile> loadPatientData(const std::string filename, const std::string password) { // reads the patient file, and loads all of the data into memory
        rapidcsv::Document file(filename);
        std::vector<PatientProfile> profiles;
        EncDec encdec;

        for (size_t i{0}; i < file.GetRowCount(); i++) {
            std::string read_profile_number{ file.GetCell<std::string>(columns.c_profile_number, i) };
            std::string read_first_name{ file.GetCell<std::string>(columns.c_first_name, i) };
            std::string read_last_name{ file.GetCell<std::string>(columns.c_last_name, i) };
            std::string read_dob{ file.GetCell<std::string>(columns.c_dob, i) };
            std::string read_reason_of_admission{ file.GetCell<std::string>(columns.c_reason_of_admission, i) };
            std::string read_time_of_admission{ file.GetCell<std::string>(columns.c_time_of_admission, i) };
            std::string read_expected_time_of_stay{ file.GetCell<std::string>(columns.c_expected_time_of_stay, i) };
            std::string read_creation_date{ file.GetCell<std::string>(columns.c_creation_date, i) };
            std::string read_admitted_unit{ file.GetCell<std::string>(columns.c_admitted_unit, i) };
            std::string read_assigned_rooms{ file.GetCell<std::string>(columns.c_assigned_rooms, i) };

            if (password != "")
            {
                read_profile_number = encdec.decrypt(password, read_profile_number);
                read_first_name = encdec.decrypt(password, read_first_name);
                read_last_name = encdec.decrypt(password, read_last_name);
                read_dob = encdec.decrypt(password, read_dob);
                read_reason_of_admission = encdec.decrypt(password, read_reason_of_admission);
                read_time_of_admission = encdec.decrypt(password, read_time_of_admission);
                read_expected_time_of_stay = encdec.decrypt(password, read_expected_time_of_stay);
                read_creation_date = encdec.decrypt(password, read_creation_date);
                read_admitted_unit = encdec.decrypt(password, read_admitted_unit);
                read_assigned_rooms = encdec.decrypt(password, read_assigned_rooms);
            }

            size_t profile_number = std::stoull(read_profile_number);
            std::string first_name = read_first_name;
            std::string last_name = read_last_name;
            time_t dob = std::stoull(read_dob);
            std::string reason_of_admission = read_reason_of_admission;
            time_t time_of_admission = stoull(read_time_of_admission);
            time_t expected_time_of_stay = stoull(read_expected_time_of_stay);
            time_t creation_date = stoull(read_creation_date);
            Unit assigned_unit = static_cast<Unit>(stoi(read_admitted_unit));
            std::vector<std::string> assigned_rooms = parseIntoRooms(read_assigned_rooms);
            profiles.emplace_back(profile_number, first_name, last_name, dob, reason_of_admission, time_of_admission, expected_time_of_stay, creation_date, assigned_unit, assigned_rooms); // creates a new object for each row, and sticks it in a vector
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
    static std::vector<StaffProfile> loadStaffData(const std::string filename, const std::string password) { // reads the staff file, and loads all of the data into memory
        EncDec encdec;
        rapidcsv::Document file(filename);
        std::vector<StaffProfile> profiles;

        for (size_t i{0}; i < file.GetRowCount(); i++) {
            std::string read_profile_number{file.GetCell<std::string>(columns.c_profile_number, i)};
            std::string read_first_name{file.GetCell<std::string>(columns.c_first_name, i)};
            std::string read_last_name{file.GetCell<std::string>(columns.c_last_name, i)};
            std::string read_dob{file.GetCell<std::string>(columns.c_dob, i)};
            std::string read_occupation{file.GetCell<std::string>(columns.c_occupation, i)};
            std::string read_wage{file.GetCell<std::string>(columns.c_shifts, i)};
            std::string read_shifts{file.GetCell<std::string>(columns.c_shifts, i)};
            std::string read_assigned_unit{file.GetCell<std::string>(columns.c_assigned_unit, i)};
            std::string read_assigned_rooms{file.GetCell<std::string>(columns.c_assigned_rooms, i)};
            std::string read_creation_date{file.GetCell<std::string>(columns.c_creation_date, i)};

            if(password != "")
            {
                read_profile_number = encdec.decrypt(password, read_profile_number);
                read_first_name = encdec.decrypt(password, read_first_name);
                read_last_name = encdec.decrypt(password, read_last_name);
                read_dob = encdec.decrypt(password, read_dob);
                read_occupation = encdec.decrypt(password, read_occupation);
                read_wage = encdec.decrypt(password, read_wage);
                read_shifts = encdec.decrypt(password, read_shifts);
                read_assigned_unit = encdec.decrypt(password, read_assigned_unit);
                read_assigned_rooms = encdec.decrypt(password, read_assigned_rooms);
                read_creation_date = encdec.decrypt(password, read_creation_date);
            }

            size_t profile_number = stoull(read_profile_number);
            std::string first_name = read_first_name;
            std::string last_name = read_last_name;
            time_t dob = stoull(read_dob);
            std::string occupation = read_occupation;
            size_t wage = stoull(read_wage);
            std::array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts = parseIntoTimeBlocks(read_shifts);
            Unit assigned_unit = static_cast<Unit>(stoi(read_assigned_unit));
            std::vector<std::string> assigned_rooms = parseIntoRooms(read_assigned_rooms);
            time_t creation_date = stoull(read_creation_date);
            profiles.emplace_back(profile_number, first_name, last_name, dob, occupation, wage, creation_date, shifts, assigned_unit, assigned_rooms);
        }
        return profiles;
    }

    /// @brief dont use this function, use savePatientData for a single patient, and saveAllPatientData for a vector of patient data 
    static void writePatientData(PatientProfile patient, std::string filename, size_t row, std::string password, EncDec encdec, rapidcsv::Document& file) {
        std::string profile_number{std::to_string(patient.getProfileNumber())};
        std::string first_name{patient.getFirstName()};
        std::string last_name{patient.getLastName()};
        std::string dob{std::to_string(patient.getDOB())};
        std::string reason{patient.getReasonOfAdmission()};
        std::string admission_time{std::to_string(patient.getTimeOfAdmission())};
        std::string expected_stay_time{std::to_string(patient.getExpectedTimeOfStay())};
        std::string creation_date{std::to_string(patient.getCreationDate())};
        std::string admitted_unit{std::to_string(patient.getAdmittedUnitInt())};
        std::string assigned_rooms{patient.getRoomString()};

        // If a password is entered, encrypt the output
        if(password != "")
        {
            profile_number = encdec.encrypt(password, profile_number);
            first_name = encdec.encrypt(password, first_name);
            last_name = encdec.encrypt(password, last_name);
            dob = encdec.encrypt(password, dob);
            reason = encdec.encrypt(password, reason);
            admission_time = encdec.encrypt(password, admission_time);
            expected_stay_time = encdec.encrypt(password, expected_stay_time);
            creation_date = encdec.encrypt(password, creation_date);
            admitted_unit = encdec.encrypt(password, admitted_unit);
            assigned_rooms = encdec.encrypt(password, assigned_rooms);
        }

        file.SetCell(0, row, profile_number);
        file.SetCell(1, row, first_name);
        file.SetCell(2, row, last_name);
        file.SetCell(3, row, dob);
        file.SetCell(4, row, reason);
        file.SetCell(5, row, admission_time);
        file.SetCell(6, row, expected_stay_time);
        file.SetCell(7, row, creation_date);
        file.SetCell(8, row, admitted_unit);
        file.SetCell(9, row, assigned_rooms);
        //file.Save();
    }

    /// @brief dont use this function, use saveStaffData for a single staff member, and saveAllStaffData for a vector of patient data 
    static void writeStaffData(StaffProfile staff, std::string filename, rapidcsv::Document& file, size_t row, std::string password, EncDec& encdec) {
        std::string profile_number{ std::to_string(staff.getProfileNumber())};
        std::string first_name{staff.getFirstName()};
        std::string last_name{staff.getLastName()};
        std::string dob{std::to_string(staff.getDOB())};
        std::string occupation{staff.getOccupation()};
        std::string wage{std::to_string(staff.getWage())};
        std::string shifts{staff.getShiftExportString()};
        std::string assigned_unit{std::to_string(staff.getAssignedUnit())};
        std::string assigned_rooms{staff.getRoomString()};
        std::string creation_date{ std::to_string(staff.getCreationDate()) };

        // If a password is entered, encrypt the output
        if (password != "")
        {
            profile_number = encdec.encrypt(password, profile_number);
            first_name = encdec.encrypt(password, first_name);
            last_name = encdec.encrypt(password, last_name);
            dob = encdec.encrypt(password, dob);
            occupation = encdec.encrypt(password, occupation);
            wage = encdec.encrypt(password, wage);
            shifts = encdec.encrypt(password, shifts);
            assigned_unit = encdec.encrypt(password, assigned_unit);
            assigned_rooms = encdec.encrypt(password, assigned_rooms);
            creation_date = encdec.encrypt(password, creation_date);
        }

        file.SetCell(0, row, profile_number);
        file.SetCell(1, row, first_name);
        file.SetCell(2, row, last_name);
        file.SetCell(3, row, dob);
        file.SetCell(4, row, occupation);
        file.SetCell(5, row, wage);
        file.SetCell(6, row, shifts);
        file.SetCell(7, row, assigned_unit);
        file.SetCell(8, row, assigned_rooms);
        file.SetCell(9, row, creation_date);
        file.Save(filename);
    }

    //========================================================================================================================================================================

    /// @brief Saves over the old data of a patient with the new data. Use for one profile
    /// @param patient The new patientProfile data. Make sure it has the same profile number as the original patient profile
    /// @param filename The name of the file to save the data to
    /// @param password The encryption password
    static void savePatientData(PatientProfile patient, std::string filename, std::string password) {
        EncDec encdec;
        rapidcsv::Document file(filename);
        if(file.GetRowCount() == 0)
        {
            for (size_t row{0}; row < file.GetRowCount(); row++) {
                if (file.GetCell<size_t>(columns.c_profile_number, row) == patient.getProfileNumber()) {
                    writePatientData(patient, filename, row, password, encdec, file);
                    return;
                }
            }
        }
        writePatientData(patient, filename, (file.GetRowCount()), password, encdec, file); // Saves to a new row
    }

    /// @brief Saves over the old data of a staff with new data. Use for one profile
    /// @param staff The new staffProfile data. Make sure that it has the same profile number as the original staff profile
    /// @param filename The name of the file to save the data to
    /// @param password The encryption password
    static void saveStaffData(StaffProfile staff, std::string filename, std::string password) {
        EncDec encdec;
        rapidcsv::Document file(filename);
        if (file.GetRowCount() == 0)
        {
            for(size_t row{0}; row < file.GetRowCount(); row++) {
                if (file.GetCell<size_t>(columns.c_profile_number, row) == staff.getProfileNumber()) {
                    writeStaffData(staff, filename, file, row, password, encdec);
                    return;
                }
            }
        }
        writeStaffData(staff, filename, file, file.GetRowCount(), password, encdec); // Saves to a new row
    }

    //========================================================================================================================================================================
    
    /// @brief Saves all of the patient data from a vector to a file.
    /// @param pProfiles A vector of patientProfiles
    /// @param filename The file to save the data to
    /// @param password The encryption password
    static void saveAllPatientData(std::vector<PatientProfile> pProfiles, std::string filename, std::string password) {
        rapidcsv::Document file(filename);
        EncDec encdec;
        for(size_t i{0}; i < pProfiles.size(); i++) {
            writePatientData(pProfiles[i], filename, i, password, encdec, file);
            file.Save();
        }
        
    }

    /// @brief Saves all of the staff data from a vector to a file.
    /// @param sProfiles A vector of staffProfiles
    /// @param filename The file to save the data to
    /// @param password The encryption password
    static void saveAllStaffData(std::vector<StaffProfile> sProfiles, std::string filename, std::string password) {
        rapidcsv::Document file(filename);
        EncDec encdec;
        for (size_t i{0}; i < sProfiles.size(); i++) {
            writeStaffData(sProfiles[i], filename, file, i, password, encdec);
            file.Save();
        }
    }

    //========================================================================================================================================================================

    /// @warning I never tested this, but it should work as intended, and we can remove this if we don't need it
    /// @brief Removes a profile from its save file, we might not need this I made it just in case things get weird
    /// @param profile_number the profile number. Can be a patient or staff profile
    /// @param filename the name of the file. Use the corresponding file for a patient or staff profile
    static void removeProfile(size_t profile_number, std::string filename) {
        rapidcsv::Document file(filename);
        for (size_t index{0}; index < file.GetRowCount(); index++) {
            if (profile_number == file.GetCell<size_t>(columns.c_profile_number, index)) {
                file.RemoveRow(index);
                return;
            }
        }
    }

};


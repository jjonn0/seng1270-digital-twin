/*
* @brief This is a file to explain the Profiles, Simulation, Filehandler, and EncDec header files.
*/
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <format>

#include "profiles.h"
#include "simulation.h"
//#include "filehandler.h"
//#include "encdec.h"

using namespace std;

int main()
{
	// PROFILES.H
	// The profiles.h file allows for an OOP representation of patient and staff profiles.
	// All profiles require a unique identifier for easier identification of similar profiles. Note that numbering of patient and staff profiles can be independent of one another.

	PatientProfile p0{0}; // Creating a patient profile with ID 0.
	StaffProfile s0(0); // Likewise for staff profile.

	cout << format("{}\n", s0.toString()); // Return a profile as a string using toString()
	// Setting the string properties of a staff member
	s0.setFirstName("Jimmy");
	s0.setLastName("Billy-Bob");
	s0.setOccupation("Doctor");
	// Setting time-sensitive properties
	time_t current_time = time_t(); // Creates a new time_t object with the current time
	// getTimestamp comes from simulation.h
	time_t january_first{getTimestamp(2026, 1, 1)}; // Creates a new time_t object with given year/month/day
	time_t staff_birth_date{getTimestamp(1990, 7, 12)};

	s0.setDOB(staff_birth_date);
	// Creating an array for shifts
	time_t shift_start{getTimestamp(2026, 4, 1, 8, 0, 0)}; // year/month/day/hour/minute/second
	time_t shift_end{getTimestamp(2026, 4, 1, 14, 30, 0)};
	TimeBlock shift_one{shift_start, shift_end}; // Create a TimeBlock struct with a given start and end time
	cout << format("The duration of the upcoming shift is: {} seconds!\n", shift_one.getTotalTime());
	array<TimeBlock, 10> upcoming_shifts{shift_one, TimeBlock{shift_start + 3600, shift_end + 3600}};
	// Setting upcoming shifts
	s0.setShifts(upcoming_shifts);

	s0.setAssignedUnit(GERIATRIC); // Here we use enumerations to determind where the staff member works. Here, we use GERIATRIC, but 6 would also work!
	s0.setWage(7200); // Setting wage to $72.00
	s0.setAssignedRooms(vector<string>{"206", "207", "208"});

	// Printing the results!
	cout << s0.toString() << endl;


	/***************************************************************************************************************************************************************/


	// SIMULATION.H
	// Simulation provides useful functions for various things!

	PatientProfile p1{1, "Kylee", "Launderdale", getTimestamp(2002, 7, 28), "Ate a lot of crayons.", getTimestamp(2026, 3, 1, 13, 0, 0), getTimestamp(2026, 3, 8), ER};
	time_t new_simulation_time{ getTimestamp(2026, 3, 5) };
	time_t old_simulation_time{getTimestamp(2026, 3, 3)};

	cout << format("\nPatient admission status {}\n0 means discharged, 1 means admitted\n\n", int(patientAdmissionStatus(p1, old_simulation_time, new_simulation_time)));

	// Estimating wage cost for s0, remember that the wage is 72.00 an hour
	cout << format("For s0's first shift, the wage cost is: {}", getCostEstimation(s0.getWage(), shift_one.getTotalTime(), HOUR));

	cout << endl << endl;

	// Creating a room
	Room r0{"200"};

	// Can set occupancy/patients/staff through constructor or seperately.
	r0.setMaxOccupancy(10);
	r0.setPatients(vector<PatientProfile>{p0, p1});
	r0.setStaff(vector<StaffProfile>{s0});

	// Current patients
	for (PatientProfile p : r0.getPatients())
	{
		cout << p.toString() << endl;
	}

	// Creating a new room to move a patient into.
	Room r1{"201"};

	// Move the patient to the new room.
	r0.movePatient(p0, r1);

	// Here is displayed that the patient was removed from r0 and moved into r1.
	cout << "Room 200\n";
	for (PatientProfile p : r0.getPatients())
	{
		cout << p.toString() << endl;
	}
	cout << "Room 201\n";
	for (PatientProfile p : r1.getPatients())
	{
		cout << p.toString() << endl;
	}

	cout << r0.getOccupancyStatus();

	/***************************************************************************************************************************************************************/


	// FILEHANDLER.H
	// Filehandler provides the necessary saving/loading.

	//FileHandler::savePatientData(p1, "examplepatients.csv", "SuperAwesomeMatrix");
	//FileHandler::saveStaffData(s0, "examplestaff.csv", "SuperAwesomeMatrix");

	//vector<PatientProfile> patients{FileHandler::loadPatientData("examplepatients.csv", "SuperAwesomeMatrix")};
	//vector<StaffProfile> staff{FileHandler::loadStaffData("examplestaff.csv", "SuperAwesomeMatrix")};

	//cout << endl << endl;
	//for(PatientProfile p : patients)
	//{
	//	cout << p.toString() << endl;
	//}

	return 0;
}
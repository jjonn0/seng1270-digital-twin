#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <format>
#include <random>
#include <ctime>
#include <algorithm>

#include "profiles.h"
#include "filehandler.h"
#include "simulation.h"
#include "encdec.h"

// Set to 1 to create test data and save to files
#define CREATE_FILES 1

using namespace std;

const size_t NUMBER_OF_PATIENT_PROFILES{100};
const size_t NUMBER_OF_STAFF_PROFILES{50};
const time_t MAX_TIME{getTimestamp(2070, 1, 1)};

random_device rd;
default_random_engine dre;

uniform_int_distribution<size_t> random_birth_year(2000, 2020);
uniform_int_distribution<size_t> random_future_year(2026, 2069);

uniform_int_distribution<size_t> random_month(1, 12);
uniform_int_distribution<size_t> random_day(1, 31);

uniform_int_distribution<size_t> random_hour(0, 23);
uniform_int_distribution<size_t> random_min_sec(0, 59);
uniform_int_distribution<int> random_unit(0, 11);

uniform_int_distribution<size_t> random_room(0, 99);

normal_distribution<double> random_symptom_count(0.0, 2.0);

vector<PatientProfile> patients;
vector<StaffProfile> staff;

// Retrieved from https://www.ssa.gov/oact/babynames/decades/century.html
const vector<string> FIRST_NAMES = { "James","Mary","Michael","Patricia","John","Jennifer","Robert","Linda","David","Elizabeth","William","Barbara","Richard","Susan","Joseph","Jessica","Thomas","Karen","Christopher","Sarah","Charles","Lisa","Daniel","Nancy","Matthew",
"Sandra","Anthony","Ashley","Mark","Emily","Steven","Kimberly","Donald","Betty","Andrew","Margaret","Joshua","Donna","Paul","Michelle","Kenneth","Carol","Kevin","Amanda","Brian","Melissa","Timothy","Deborah","Ronald","Stephanie","Jason","Rebecca","George","Sharon","Edward",
"Laura","Jeffrey","Cynthia","Ryan","Amy","Jacob","Kathleen","Nicholas","Angela","Gary","Dorothy","Eric","Shirley","Jonathan","Emma","Stephen","Brenda","Larry","Nicole","Justin","Pamela","Benjamin","Samantha","Scott","Anna","Brandon","Katherine","Samuel","Christine",
"Gregory","Debra","Alexander","Rachel","Patrick","Olivia","Frank","Carolyn","Jack","Maria","Raymond","Janet","Dennis","Heather","Tyler","Diane","Aaron","Catherine","Jerry","Julie","Jose","Victoria","Nathan","Helen","Adam","Joyce","Henry","Lauren","Zachary","Kelly",
"Douglas","Christina","Peter","Joan","Noah","Judith","Kyle","Ruth","Ethan","Hannah""Christian","Evelyn","Jeremy","Andrea","Keith","Virginia","Austin","Megan","Sean","Cheryl","Roger","Jacqueline","Terry","Madison","Walter","Sophia","Dylan","Abigail","Gerald","Teresa",
"Carl","Isabella","Jordan","Sara","Bryan","Janice","Gabriel","Martha","Jesse","Gloria","Harold","Kathryn","Lawrence","Ann","Logan","Charlotte","Arthur","Judy","Bruce","Amber","Billy","Julia","Elijah","Grace","Joe","Denise","Alan","Danielle","Juan","Natalie","Liam",
"Alice","Willie","Marilyn","Mason","Diana","Albert","Beverly","Randy","Jean","Wayne","Brittany","Vincent","Theresa","Lucas","Frances","Caleb","Kayla","Luke","Alexis","Bobby","Tiffany","Isaac","Lori","Bradley","Kathy"};

// Retrieved from https://probablyhelpful.com/most_common_surnames.htm
const vector<string> LAST_NAMES = {"SMITH","JOHNSON","WILLIAMS","JONES","BROWN","DAVIS","MILLER","WILSON","MOORE","TAYLOR","ANDERSON","THOMAS","JACKSON","WHITE","HARRIS","MARTIN","THOMPSON","GARCIA","MARTINEZ","ROBINSON","CLARK","RODRIGUEZ","LEWIS","LEE","WALKER","HALL",
"ALLEN","YOUNG","HERNANDEZ","KING","WRIGHT","LOPEZ","HILL","SCOTT","GREEN","ADAMS","BAKER","GONZALEZ","NELSON","CARTER","MITCHELL","PEREZ","ROBERTS","TURNER","PHILLIPS","CAMPBELL","PARKER","EVANS","EDWARDS","COLLINS","STEWART","SANCHEZ","MORRIS","ROGERS","REED","COOK",
"MORGAN","BELL","MURPHY","BAILEY","RIVERA","COOPER","RICHARDSON","COX","HOWARD","WARD","TORRES","PETERSON","GRAY","RAMIREZ","JAMES","WATSON","BROOKS","KELLY","SANDERS","PRICE","BENNETT","WOOD","BARNES","ROSS","HENDERSON","COLEMAN","JENKINS","PERRY","POWELL","LONG",
"PATTERSON","HUGHES","FLORES","WASHINGTON","BUTLER","SIMMONS","FOSTER","GONZALES","BRYANT","ALEXANDER","RUSSELL","GRIFFIN","DIAZ","HAYES"};

// Retrieved from https://scer247.com/blog/common-reasons-for-er-visits/
const vector<string> ADMISSION_REASONS = {"Acute seizure occurance.", "Coughing up blood.", "Acute jaundice.", "High fever.", "Acute abdominal pain.", "Thunderclap headache.", "Severe chet pain.", "Wheezing cough.", "Acute shortness of breath.",
"Acute back pain.", "Acute leg pain.", "Heart attack.", "Cardiac arrest.", "Broken arm.", "Broken leg.", "Asthma attack.", "Stopped breathing."};

string getRandomName(const vector<string> name_list)
{
	uniform_int_distribution<size_t> dist(0, name_list.size() - 1);
	return name_list[dist(dre)];
}

time_t getRandomDOB()
{
	return getTimestamp(random_birth_year(dre), random_month(dre), random_day(dre), random_hour(dre), 0, 0);
}

TimeBlock getRandomBlock()
{
	time_t time_start{getTimestamp(random_future_year(dre), random_month(dre), random_day(dre), random_hour(dre), 0, 0)};
	time_start = clamp(time_start, time(nullptr), MAX_TIME);
	TimeBlock t{time_start, time_start + TimeUnit::HOUR * 8};
	return t;
}

string getRandomRoom(Unit unit)
{
	return to_string(random_room(dre) + unit * 100);
}

string getRandomSymptoms(size_t count)
{
	string symptom_list{""};
	uniform_int_distribution<size_t> dist(0, ADMISSION_REASONS.size() - 1);
	while(count > 0)
	{
		symptom_list += ADMISSION_REASONS[dist(dre)];
		if(count > 1)
		{
			symptom_list + " ";
		}
		count--;
	}
	return symptom_list;
}

int main()
{
	time_t time_start, time_end;

#if CREATE_FILES == 1
	// Generate patient profiles
	for(size_t profile_count{0}; profile_count < NUMBER_OF_PATIENT_PROFILES; profile_count++)
	{
		PatientProfile new_profile{profile_count};
		new_profile.setFirstName(getRandomName(FIRST_NAMES));
		new_profile.setLastName(getRandomName(LAST_NAMES));
		time_t dob{getRandomDOB()};
		new_profile.setDOB(getRandomDOB());
		time_t admission_time{clamp(time_t(random_future_year(dre)) + (dob / 2), dob, MAX_TIME)};
		new_profile.setReasonOfAdmission(getRandomSymptoms(1));
		new_profile.setTimeOfAdmission(admission_time);
		new_profile.setExpectedTimeOfStay(clamp(int64_t(random_future_year(dre)), admission_time + 3600, MAX_TIME));
		new_profile.setAssignedUnit(static_cast<Unit>(random_unit(dre)));
		new_profile.setAssignedRooms(vector<string>{getRandomRoom(new_profile.getAssignedUnit())});

		patients.push_back(new_profile);
	}

	// Generate staff profiles
	for(size_t profile_count{0}; profile_count < NUMBER_OF_STAFF_PROFILES; profile_count++)
	{
		StaffProfile new_profile{profile_count};
		new_profile.setFirstName(getRandomName(FIRST_NAMES));
		new_profile.setLastName(getRandomName(LAST_NAMES));
		time_t dob{getRandomDOB()};
		new_profile.setDOB(dob);
		new_profile.setWage(uniform_int_distribution<size_t>(0, 10000)(dre));
		array<TimeBlock, MAXIMUM_STORED_SHIFTS> timeblocks{};
		for(size_t timeblock_count{0}; timeblock_count < MAXIMUM_STORED_SHIFTS; timeblock_count++)
		{
			timeblocks[timeblock_count] = getRandomBlock();
		}
		new_profile.setShifts(timeblocks);
		new_profile.setAssignedUnit(static_cast<Unit>(random_unit(dre)));
		vector<string> rooms{};
		for(size_t room_count{0}; room_count < 5; room_count++)
		{
			bool existing_room{false};
			string room{getRandomRoom(new_profile.getAssignedUnit())};
			for(string r : rooms)
			{
				if(room == r)
				{
					existing_room = true;
				}
			}
			if(!existing_room)
			{
				rooms.push_back(room);
			}
		}
		new_profile.setAssignedRooms(rooms);
		staff.push_back(new_profile);
	}

	time_start = time(nullptr);
	cout << "Generating unencrypted patient profiles...\n";
	FileHandler::saveAllPatientData(patients, "TestPatients.csv", "");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Generating encrypted patient profiles...\n";
	FileHandler::saveAllPatientData(patients, "TestPatientsEnc.csv", "Password");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Generating unencrypted staff profiles...\n";
	FileHandler::saveAllStaffData(staff, "TestStaff.csv", "");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Generating encrypted staff profiles...\n";
	FileHandler::saveAllStaffData(staff, "TestStaffEnc.csv", "Password");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

#endif CREATE_FILES

	time_start = time(nullptr);
	cout << "Reading unencrypted patient profiles...\n";
	vector<PatientProfile> p{FileHandler::loadPatientData("TestPatients.csv", "")};
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Reading encrypted patient profiles...\n";
	FileHandler::loadPatientData("TestPatientsEnc.csv", "Password");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Reading unencrypted staff profiles...\n";
	FileHandler::loadStaffData("TestStaff.csv", "");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	time_start = time(nullptr);
	cout << "Reading encrypted staff profiles...\n";
	FileHandler::loadStaffData("TestStaffEnc.csv", "Password");
	time_end = time(nullptr);
	cout << "The operation took " << (time_end - time_start) << "s\n";

	return 0;
}
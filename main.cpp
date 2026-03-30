#include"filehandler.h"

using namespace std;

int main() {
    array<TimeBlock, MAXIMUM_STORED_SHIFTS> shifts = {};
    vector<std::string> rooms = {"room 1", "room 2"};
    StaffProfile staff(10, "Jonathon", "Oldegbers", 13, "silly guy", 12, 1, shifts, Unit::ER, rooms);
    FileHandler::saveStaffData(staff, "examplestaffprofile.csv");
    return 0;
}
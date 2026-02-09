#include <iostream>
#include <chrono>
#include <typeinfo>
#include "timetracker.h"
#include "timereference.h"
#include "filehandler.h"
#include "record.h"
using namespace std;

size_t seconds_since_unix; // Seconds since UNIX epoch was established

void exampleClock();

int main()
{
    //exampleClock();

    get_unix_time("14s:33m:22h:08D:02M:2026Y");

    return 0;
}

void exampleClock()
{
    update_seconds_since_unix(&seconds_since_unix);
    TimeTracker t1 = TimeTracker(&seconds_since_unix, 5);
    while(!t1.isPassed())
    {
        cout << t1.getTimeStamp() << endl;
        update_seconds_since_unix(&seconds_since_unix);
    }
}
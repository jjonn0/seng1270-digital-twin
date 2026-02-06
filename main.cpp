#include <iostream>
#include <chrono>
#include <typeinfo>
#include "TimeReference.h"
using namespace std;

size_t seconds_since_unix; // Seconds since UNIX epoch was established

int main()
{
    size_t time{120};
    update_seconds_since_unix(&seconds_since_unix);
    TimeReference t1 = TimeReference(&seconds_since_unix, 5);
    while(!t1.isPassed())
    {
        cout << t1.getTimeStamp() << endl;
        update_seconds_since_unix(&seconds_since_unix);
    }
    return 0;
}
#include <iostream>
#include <chrono>
#include <typeinfo>
#include "TimeReference.h"
using namespace std;

size_t seconds_since_unix; // Seconds since UNIX epoch was established

int main()
{
    update_seconds_since_unix(&seconds_since_unix);
    TimeReference t1 = TimeReference(&seconds_since_unix, 10);
    TimeReference t2 = TimeReference(&seconds_since_unix, 2000000000);
    cout << t1.getRemainingSeconds() << endl;
    cout << t2.getRemainingSeconds() << endl;
    int x;
    cin >> x;
    update_seconds_since_unix(&seconds_since_unix);
    cout << t1.getRemainingSeconds() << endl;
    cout << t2.getRemainingSeconds() << endl;
    cout << t2.getTimeStamp();
    return 0;
}
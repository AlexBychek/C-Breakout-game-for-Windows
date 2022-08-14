//
// Created by https://github.com/AlexBychek
//

#include <DateTime.h>

#include <chrono>

using namespace std;

uint64_t getMills()
{
    return chrono::duration_cast< chrono::milliseconds >( chrono::system_clock::now().time_since_epoch()).count();
}

std::string getDateTime()
{
    auto dateTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm calendar;

    localtime_s(&calendar, &dateTime);

    std::string strCalendar = to_string(calendar.tm_year + 1900) + "-"
                              + ((calendar.tm_mon + 1) < 10 ? string("0" + to_string(calendar.tm_mon + 1)) : to_string(calendar.tm_mon + 1 )) + "-"
                              + ((calendar.tm_mday) < 10 ? string("0" + to_string(calendar.tm_mday)) : to_string(calendar.tm_mday )) + " "
                              + ((calendar.tm_hour) < 10 ? string("0" + to_string(calendar.tm_hour)) : to_string(calendar.tm_hour )) + ":"
                              + ((calendar.tm_min) < 10 ? string("0" + to_string(calendar.tm_min)) : to_string(calendar.tm_min )) + ":"
                              + ((calendar.tm_sec) < 10 ? string("0" + to_string(calendar.tm_sec)) : to_string(calendar.tm_sec ));

    return strCalendar;
}

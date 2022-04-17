#include "pch.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <sstream>
#include "instrumentation.h"
#include "util.h"
std::unordered_map<std::string, Instrumentation> Instrumentation::timers;
std::atomic<double> Instrumentation::instrument_time;

using namespace std;
//string ltrim(const string& str) {
//    return regex_replace(str, regex("^\\s+"), string(""));
//}
//
//string rtrim(const string& str) {
//    return regex_replace(str, regex("\\s+$"), string(""));
//}
//
//string trim(const std::string& str) {
//    return ltrim(rtrim(str));
//}

ostream& operator<<(ostream& os, const duration<double, std::nano>& total_time) {
    milliseconds milli = duration_cast<milliseconds>(total_time);
    stringstream ss;
    string units;

    if (milli.count() > 2000) {
        units = " seconds";
        ss << setprecision(3) << fixed << right << milli.count() / 1000.0;
    }
    else {
        milliseconds milli = duration_cast<milliseconds>(total_time);
        ss.precision(3);
        if (milli.count() > 10) {
            units = " ms";
            ss << milli.count();
        }
        else {
            units = " nanoseconds";
            ss << fixed << right << total_time.count();
        }
    }
    os << right << setw(15) << ss.str() << setw(12) << left << units;;
    return os;
}

ostream& operator<<(ostream& os, const Instrumentation& item) {
    using namespace std;
    duration<double, std::nano> total_time(std::chrono::nanoseconds(item.total_time));

    long count = item.count;
    os << total_time << " " << right << setw(10) << item.count << left << " " << (total_time / count);
    return os;
}

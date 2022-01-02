#include "pch.h"
#include <regex>
#include "util.h"

using namespace std;
string ltrim(const string& str) {
    return regex_replace(str, regex("^\\s+"), string(""));
}

string rtrim(const string& str) {
    return regex_replace(str, regex("\\s+$"), string(""));
}

string trim(const std::string& str) {
    return ltrim(rtrim(str));
}

bool contains(std::string& str, char c) {
    return (str.find(c) != std::string::npos);
}

#include "utils/time.h"

#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

std::string ezserver::utils::time::GetTimeString(const time_t& time, const std::string& format) {
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), format.c_str());
    return ss.str();
}
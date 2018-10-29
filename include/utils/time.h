#ifndef EZSERVER_TIME_H
#define EZSERVER_TIME_H

#include <string>
#include <ctime>

namespace ezserver::utils::time
{
    /**
     * Gets the time string out of time_t object
     * in a specific format
     */
    std::string GetTimeString(const std::time_t &time, const std::string& format);
}

#endif //EZSERVER_TIME_H

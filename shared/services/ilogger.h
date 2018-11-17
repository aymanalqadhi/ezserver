#ifndef EZSERVER_ILOGGER_H
#define EZSERVER_ILOGGER_H

#include "services/iservice.h"
#include <ostream>
#include <atomic>

/// Services namespace
namespace ezserver::shared::services
{
    /// Service unique id
    const std::string kLoggerService = "eztech.com/ezserver/services/loggor";

    /**
     * An enum to represent the application logging level
     *
     * kFatal       Fatal unrecoverable error
     * kError       Recoverable error
     * kWarning     Something that might cause errors
     * kInformation Normal state
     * kDebug       More verbouse messages, used for debugging
     * kTrace       The trace calls of exceptions
     */
    enum class LoggingLevel : int
    {
        kFatal = 0, kError = 1, kWarning = 2,
        kInformation = 3, kDebug = 4, kTrace = 5
    };

    /**
     * Loggers Base class
     */
    class ILogger : public ezserver::shared::services::IService
    {
    public:
        /**
         * Logs a messgae to an output stream
         * @param level The logging level
         * @return The stream logged into
         */
        virtual std::ostream& Log(const LoggingLevel& level) = 0;

        /**
         * Gets whether the user has selected to
         * include the time with the log or not
         *
         * @return The time including status
         */
        const bool IncludeTime() const { return this->include_time_; }

        /**
         * Sets wehter the logger should contain time
         * with message or not
         *
         * @param new_value The new time including status value
         */
        const void IncludeTime(bool new_value) { this->include_time_.store(new_value); }

    protected:
        /// If true, the log messages will contain the current timestamp
        std::atomic_bool include_time_;
    };
}

/**
 * A Macro to ease the use of the logging function
 * provived by ILogger services
 */
#define LOG(logger, mode) logger->Log(ezserver::shared::services::LoggingLevel::k##mode)

#endif //EZSERVER_ILOGGER_H

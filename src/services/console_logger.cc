#include "services/console_logger.h"
#include "utils/time.h"

#include <termcolor/termcolor.hpp>
#include <ostream>
#include <ctime>

using ezserver::shared::services::LoggingLevel;

// ====================================================== //

std::ostream& ezserver::services::ConsoleLogger::Log(const LoggingLevel &level)
{
    // Gets the timestamp string
    auto timestamp = this->IncludeTime() ? "[" + ezserver::utils::time::GetTimeString(time(0), "%H:%M:%S") + "]" : "";

    switch(level)
    {
        // Trace level: used to log exception trace calls and messages
        case LoggingLevel::kTrace:
            return this->writing_stream_ << termcolor::grey << timestamp << "[Trace] " << termcolor::reset;

        // Debug level: used for debugging purposes
        case LoggingLevel::kDebug:
            return this->writing_stream_ << termcolor::grey << termcolor::bold << timestamp << "[Debug] " << termcolor::reset;

        // Information level: used to log usual messages
        case LoggingLevel::kInformation:
            return this->writing_stream_ << termcolor::cyan << termcolor::bold << timestamp << "[Info]  " << termcolor::reset;

        // Warning level: used to log warning messages
        case LoggingLevel::kWarning:
            return this->writing_stream_ << termcolor::yellow << termcolor::bold << timestamp << "[Warn]  " << termcolor::reset;

        // Error level: used to log recoverable errors messages
        case LoggingLevel::kError:
            return this->writing_stream_ << termcolor::red << timestamp << "[Error] " << termcolor::reset;

        // Fatal level: used to log unrecvoerable erros messaegs
        case LoggingLevel::kFatal:
            return this->writing_stream_ << termcolor::red << termcolor::bold << timestamp << "[Fatal] " << termcolor::reset;
    }

    return this->writing_stream_;
}

// ====================================================== //

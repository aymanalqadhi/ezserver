#include "services/console_logger.h"
#include "utils/time.h"

#include <termcolor/termcolor.hpp>
#include <ostream>
#include <ctime>

using ezserver::shared::services::LoggingLevel;

std::ostream& ezserver::services::ConsoleLogger::Log(const LoggingLevel &level)
{
    auto timestamp = this->IncludeTime() ? "[" + ezserver::utils::time::GetTimeString(time(0), "%H:%M:%S") + "]" : "";

    switch(level)
    {
        case LoggingLevel::kTrace:
            return this->writing_stream_ << termcolor::grey << timestamp << "[Trace] " << termcolor::reset;

        case LoggingLevel::kDebug:
            return this->writing_stream_ << termcolor::grey << termcolor::bold << timestamp << "[Debug] " << termcolor::reset;

        case LoggingLevel::kInformation:
            return this->writing_stream_ << termcolor::cyan << termcolor::bold << timestamp << "[Info]  " << termcolor::reset;

        case LoggingLevel::kWarning:
            return this->writing_stream_ << termcolor::yellow << termcolor::bold << timestamp << "[Warn]  " << termcolor::reset;

        case LoggingLevel::kError:
            return this->writing_stream_ << termcolor::red << timestamp << "[Error] " << termcolor::reset;

        case LoggingLevel::kFatal:
            return this->writing_stream_ << termcolor::red << termcolor::bold << timestamp << "[Fatal] " << termcolor::reset;
    }

    return this->writing_stream_;
}
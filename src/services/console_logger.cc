#include "services/console_logger.h"
#include "utils/time.h"

#include <termcolor/termcolor.hpp>
#include <ostream>
#include <ctime>

using ezserver::shared::services::LoggingLevel;

std::ostream& ezserver::services::ConsoleLogger::Log(const LoggingLevel &level) {

    std::string timestamp = this->IncludeTime() ? "[" + ezserver::utils::time::GetTimeString(time(0), "%H:%M:%S") + "]" : "";

    switch(level)
    {
        case LoggingLevel::kTrace:
            *this->writing_stream_ << termcolor::grey << timestamp << "[Trace] " << termcolor::reset;
            break;

        case LoggingLevel::kDebug:
            *this->writing_stream_ << termcolor::grey << termcolor::bold << timestamp << "[Debug] " << termcolor::reset;
            break;

        case LoggingLevel::kInformation:
            *this->writing_stream_ << termcolor::cyan << termcolor::bold << timestamp << "[Info] " << termcolor::reset;;
            break;

        case LoggingLevel::kWarning:
            *this->writing_stream_ << termcolor::yellow << termcolor::bold << timestamp << "[Warning] " << termcolor::reset;;;
            break;

        case LoggingLevel::kError:
            *this->writing_stream_ << termcolor::red << timestamp << "[Error] " << termcolor::reset;
            break;

        case LoggingLevel::kFatal:
            *this->writing_stream_ << termcolor::red << termcolor::bold << timestamp << "[Fatal] " << termcolor::reset;;
            break;
    }

    return *this->writing_stream_;
}
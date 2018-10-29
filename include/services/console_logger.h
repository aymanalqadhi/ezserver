#ifndef EZSERVER_CONSOLE_LOGGER_H
#define EZSERVER_CONSOLE_LOGGER_H

#include "services/ilogger.h"
#include <boost/di.hpp>
#include <iostream>

/// Services Namespace
namespace ezserver::services
{
    /**
     * A Logger that logs into the console
     */
    class ConsoleLogger : public ezserver::shared::services::ILogger {
    public:

        /// Base class method override
        const virtual std::string Name() const noexcept override { return "ConsoleLogger"; }

        /// Base class method override
        virtual bool Initialize() override { return true; }

        /// Base class method override
        virtual std::ostream& Log(const ezserver::shared::services::LoggingLevel& level) override;

        /**
         * Default constructor.
         * Sets the writing stream to the standard output stream
         */
         ConsoleLogger() : writing_stream_(&std::cout) {
             IncludeTime(true);
         }

    private:
        /// The stream to write to
        std::ostream* writing_stream_;
    };

    /**
     * A Lambda to export this module
     */
    auto consolelogger_module = [] {
        return boost::di::make_injector(
            boost::di::bind<ezserver::shared::services::ILogger>.to<ConsoleLogger>()
        );
    };
}

#endif //EZSERVER_CONSOLE_LOGGER_H

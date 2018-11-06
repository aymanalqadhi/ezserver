#include <config/configuration.h>
#include <config/named_config.h>
#include <config/constants.h>

#include <application.h>
#include <bootstrapper.h>

#include <services/services_manager.h>
#include <services/console_logger.h>
#include <services/filesystem.h>

#include <net/async_tcp_listener.h>
#include <introp/plugins_loader.h>

#include <termcolor/termcolor.hpp>
#include <boost/asio/io_context.hpp>

/// Parses the command line & configuartion file options
std::shared_ptr<boost::program_options::variables_map> ParseOptions(const int /* argc */, const char *[] /* argv */);

// ================================================================ //

int main(int argc, const char *argv[])
{
    // Process command line arguments
    auto map_ptr = ParseOptions(argc, argv);
    boost::asio::io_context io;

    // Create the DI injector
    const auto inj = boost::di::make_injector(
        // Binds the main app configuration
        boost::di::bind<boost::program_options::variables_map>.to(map_ptr),
        boost::di::bind<boost::asio::io_context>.to(io),

        ezserver::introp::plugins_loader_module(),
        ezserver::services::services_manager_module(),
        ezserver::services::consolelogger_module(),
        ezserver::services::filesystem_module(),
        ezserver::application_module(),
        ezserver::net::async_tcp_listener_module(),
        ezserver::config::named::config_module(*map_ptr)
    );

    // Show the splash screen
    std::cout << termcolor::blue << std::endl << APP_SPLASH_LOGO << std::endl
              << termcolor::bold << APP_SPLASH_CAPTION << termcolor::reset << std::endl << std::endl;


    // A holder pointer for the application logger
    std::shared_ptr<ezserver::shared::services::ILogger> logger;

    // Try to run the application bootstrapper, and catch any unhandled exception
    // and exit after showing a message to the user through the logger
    try
    {
        // Try to create the application bootstrapper from the DI injector
        auto bootstrapper = inj.create<ezserver::Bootstrapper>();
        logger = inj.create<std::shared_ptr<ezserver::shared::services::ILogger>>();

        // Run the application's bootstrapper
        bootstrapper.Run();

        // Dummy stop
        int x; std::cin >> x;
    }
    catch (const std::exception &ex)
    {
        if (logger != nullptr)
            logger->Log(ezserver::shared::services::LoggingLevel::kFatal) << ex.what() << std::endl;
        else
            std::cout << termcolor::red << termcolor::bold << "[Fatal] " << termcolor::reset << ex.what() << std::endl;

        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

// ================================================================ //

/**
 * Parses the application options
 *
 * @param argc  The application arguments count
 * @param argv  The application arguemtns array
 *
 * @return The parsed map
 */
std::shared_ptr<boost::program_options::variables_map> ParseOptions(const int argc, const char *argv[])
{
    auto options = ezserver::config::GetOptionsAggrgator();
    auto map = std::make_shared<boost::program_options::variables_map>();

    try
    {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, *options), *map);
        boost::program_options::notify(*map);

        if (map->count("help"))
        {
            std::cout << *options;
            exit(EXIT_SUCCESS);
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what();
        exit(EXIT_FAILURE);
    }

    return map;
}

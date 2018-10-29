#include "application.h"

#include <termcolor/termcolor.hpp>
#include <iostream>

bool ezserver::Application::Startup()
{
    // Show welcome message
    std::cout << std::endl << termcolor::cyan << "\t-<( Welcome to EZ-SERVER! )>-" << std::endl << std::endl;

    
    return true;
}


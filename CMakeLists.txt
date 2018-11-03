cmake_minimum_required(VERSION 2.8)

# Set Project's Name
project(ezserver)

# Set Flags
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")

# Set Directories
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH  ${CMAKE_BINARY_DIR}/plugins)

# Include Libraries
include_directories(include shared)

# Add Source files
set(SOURCE_FILES src/main.cc src/bootstrapper.cc src/application.cc src/services/console_logger.cc src/services/services_manager.cc src/services/filesystem.cc src/introp/plugins_loader.cc src/config/configuration.cc)
set(INCLUDE_FILES include/config/configuration.h include/services/console_logger.h include/services/filesystem.h include/config/named_config.h)
set(SHARED_FILES shared/introp/iplugin.h shared/services/iservice.h include/services/services_manager.h shared/services/ilogger.h include/bootstrapper.h shared/iapplication.h shared/ibootstrapper.h include/application.h include/utils/time.h src/utils/time.cc include/introp/plugins_loader.h shared/services/ifilesystem.h shared/introp/iplugins_loader.h include/config/constants.h shared/net/itcp_listener.h shared/utils/event_handler.hpp shared/net/itcp_client.h)

# Definitions
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

add_executable(ezserver ${SOURCE_FILES} ${INCLUDE_FILES} ${SHARED_FILES})

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(REQUIRED_LIBRARIES pthread dl)
target_link_libraries(ezserver ${Boost_LIBRARIES} ${REQUIRED_LIBRARIES})


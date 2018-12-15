#include "services/filesystem.h"

#include <utils/time.h>

#include <boost/filesystem.hpp>
#include <thread>

# ifdef BOOST_POSIX_API
#   define COM_NAME        "ezTech"
#   define APP_NAME        "ezServer"
#   define APPDATA_ENV_VAR "HOME"
# else
#   define COM_NAME        "EZ-TECH"
#   define APP_NAME        "EZ-SERVER"
#   define APPDATA_ENV_VAR "APP_DATA"
# endif

// ====================================================== //

bool ezserver::services::Filesystem::Initialize()
{
    // Declare the app main directory
    app_main_dir_ = boost::filesystem::path(getenv(APPDATA_ENV_VAR)) / COM_NAME;

    // A Lambda to handle directory creation process
    const auto create_dir = [&](const boost::filesystem::path &path) {
        if (!boost::filesystem::exists(path))
        {
            std::cout << termcolor::bold << termcolor::grey
                      << ezserver::utils::time::GetTimeString(std::time(0), "[%H:%M:%S] ") << termcolor::reset
                      << "Creating " << path << "... " << std::endl;

            if (!boost::filesystem::create_directory(path))
                throw std::runtime_error("Could not create " + path.string());
        }
    };

    // Create app directories
    create_dir(app_main_dir_);
    create_dir(app_main_dir_ /= APP_NAME);

    // Create app sub directories
    const auto required_directories = {"plugins", "data"};
    for (auto &&dir : required_directories)
        create_dir(app_main_dir_ / dir);

    return true;
}

// ====================================================== //

boost::filesystem::path ezserver::services::Filesystem::GetPath(const ezserver::services::CommonDirectories &cd)
{
    // TODO:
    // * Make a cross-platform solution for this

    switch (cd)
    {
        case ezserver::shared::services::CommonDirectories::Data:
            return app_main_dir_ / "data";
        case ezserver::shared::services::CommonDirectories::Plugins:
            return app_main_dir_ / "plugins";
        default:
            return app_main_dir_;
    }
}

// ====================================================== //

std::unique_ptr<std::fstream> ezserver::services::Filesystem::OpenFile(
    const ezserver::services::CommonDirectories &cd,
    const boost::filesystem::path &path,
    const ezserver::services::FileMode &mode
) {
    return std::make_unique<std::fstream>((GetPath(cd) / path).string());
}

// ====================================================== //

bool ezserver::services::Filesystem::DeleteFile(
    const ezserver::services::CommonDirectories &cd,
    const boost::filesystem::path &filename
) {
    // Try to remove the file, if failed, log the error message and return false
    try
    {
        boost::filesystem::remove(GetPath(cd) / filename);
        return true;
    }
    catch (const std::exception &ex)
    {
        if (logger_->IsBootstrapped()) LOG(logger_, Error) << ex.what() << std::endl;
        return false;
    }
}

// ====================================================== //

bool ezserver::services::Filesystem::RenameFile(
    const ezserver::services::CommonDirectories &cd,
    const boost::filesystem::path &old_name,
    const boost::filesystem::path &new_name
) {
    // Try to rename the file, if failed, log the error message and return false
    try
    {
        boost::filesystem::rename(GetPath(cd) / old_name, GetPath(cd) / new_name);
        return true;
    }
    catch (const std::exception &ex)
    {
        if (logger_->IsBootstrapped()) LOG(logger_, Error) << ex.what() << std::endl;
        return false;
    }
}

// ====================================================== //

std::vector<boost::filesystem::path> ezserver::services::Filesystem::ListDirectory(
    const ezserver::services::CommonDirectories &cd
) {
    // Return an empty vector if the path doesn't exist
    if (!boost::filesystem::exists(GetPath(cd))) return {};

    // An empty return vector
    std::vector<boost::filesystem::path> ret;

    // Loop through items in the directory, and add them to the return vector
    for (auto &&p : boost::filesystem::directory_iterator(GetPath(cd)))
        ret.push_back(p.path());

    // return the return vector
    return ret;
}
#ifndef EZSERVER_IFILESYSTEM_SERVICE_H
#define EZSERVER_IFILESYSTEM_SERVICE_H

#include <services/iservice.h>

#include <boost/filesystem/path.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

/// Shared services namespace
namespace ezserver::shared::services
{
    /// Service unique id
    const std::string kFilesystemService = "eztech.com/ezserver/services/filesystem";

    /// An enum to represent the common app directories
    enum class CommonDirectories {
        AppDirectory, Plugins, Data
    };

     /// The mode to open the file in
     enum class FileMode {
        Create, CreateOrAppend, Append
    };

     /// The base class for the FilesystemService
    class IFilesystem : public IService {
    public:

        /**
         * Gets the path string out of CommonDirectories enum
         * @param  cd The common directory value
         * @return The path is string format
         */
        virtual boost::filesystem::path GetPath(const CommonDirectories& cd) = 0;

        /**
         * Opens or creates a files
         * @param cd    The parent common directory
         * @param path  The file path
         * @param mode  The opening mode
         * @return      A files stream to the file
         * @throws std::rnutime_error if there was a problem opening or creating the file
         */
        virtual std::unique_ptr<std::fstream> OpenFile(const CommonDirectories& cd, const boost::filesystem::path& path, const FileMode& mode = FileMode::CreateOrAppend) = 0;

        /**
         * Deletes a file
         * @param cd        The parent common directory
         * @param filename  The file name
         * @return          The operation result
         */
        virtual bool DeleteFile(const CommonDirectories& cd, const boost::filesystem::path& filename) = 0;

        /**
         * Renames a file
         * @param cd        The parent common direcotry
         * @param old_name  The old file name
         * @param new_name  The new file name
         * @return          The operation result
         */
        virtual bool RenameFile(const CommonDirectories& cd, const boost::filesystem::path& old_name, const boost::filesystem::path& new_name) = 0;

        /**
         * Lists a directory entries
         * @param  cd The directory to list its contents
         * @return The directories entries list
         */
        virtual std::vector<boost::filesystem::path> ListDirectory(const CommonDirectories& cd) = 0;
    };
}

#endif //EZSERVER_IFILESYSTEM_SERVICE_H

#ifndef EZSERVER_FILESYSTEM_SERVICE_H
#define EZSERVER_FILESYSTEM_SERVICE_H

#include "services/ifilesystem.h"

#include <services/ilogger.h>
#include <boost/di.hpp>
#include <boost/filesystem/path.hpp>

namespace ezserver::services
{
    using CommonDirectories = ezserver::shared::services::CommonDirectories;
    using FileMode = ezserver::shared::services::FileMode;

    class Filesystem : public ezserver::shared::services::IFilesystem {
    public:
        /// Base class method override
        virtual const std::string Name() const noexcept override
        { return "FileSystem"; }

        /// Base class method override
        virtual const bool IsRequired() const noexcept override
        { return true; }


        /// Base class method override
        virtual boost::filesystem::path GetPath(const CommonDirectories &cd) override;

        /// Base class method override
        virtual std::unique_ptr<std::fstream> OpenFile(
            const CommonDirectories &cd, const boost::filesystem::path &path,
            const FileMode &mode = FileMode::CreateOrAppend
        ) override;

        /// Base class method override
        virtual bool DeleteFile(const CommonDirectories &cd, const boost::filesystem::path &filename) override;

        /// Base class method override
        virtual bool RenameFile(
            const CommonDirectories &cd, const boost::filesystem::path &old_name,
            const boost::filesystem::path &new_name
        ) override;

        /// Base class method override
        virtual std::vector<boost::filesystem::path> ListDirectory(const CommonDirectories &cd) override;

        /// Default constructor
        BOOST_DI_INJECT(
            Filesystem,
            const std::shared_ptr<ezserver::shared::services::ILogger> &logger
        ) : logger_(logger) {}

    protected:
        /// Base class method override
        virtual bool Initialize() override;

    private:
        /// A Service to manage app logs
        std::shared_ptr<ezserver::shared::services::ILogger> logger_;

        /// The application direcotry
        boost::filesystem::path app_main_dir_;
    };

    /**
     * A Lambda to get this module
     */
    auto filesystem_module = [] {
        return boost::di::make_injector(
                boost::di::bind<ezserver::shared::services::IFilesystem>.to<Filesystem>()
        );
    };
}

#endif //EZSERVER_FILESYSTEM_SERVICE_H

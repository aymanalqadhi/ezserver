#ifndef EZSERVER_PLUGINS_LOADER_H
#define EZSERVER_PLUGINS_LOADER_H

#include "introp/iplugins_loader.h"

#include <introp/iplugin.h>
#include <services/ilogger.h>
#include <services/ifilesystem.h>

#include <boost/di.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/program_options/variables_map.hpp>

#include <memory>
#include <map>

/// Introp Namespace
namespace ezserver::introp
{
    class PluginsLoader : public ezserver::shared::introp::IPluginsLoader {
    public:

        /**
         * Default dependency injection constructor
         */
        BOOST_DI_INJECT(PluginsLoader,
            boost::program_options::variables_map& options,
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            const std::shared_ptr<ezserver::shared::services::IFilesystem>& filesystem)
        : options_(options), logger_(logger), filesystem_(filesystem) {}

        /// Base class method override
        virtual void LoadPlugins(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins
        ) override;

        /// Base class method override
        virtual void InitializePlugins(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins,
            std::shared_ptr<ezserver::shared::services::IServicesManager>& services_manager
        ) override;

        /// Base class method override
        virtual void LoadCommands(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins,
            std::unordered_map<std::string, ezserver::shared::introp::ExportedCommand> &commands
        ) override;

    private:
        /// The options of the application
        boost::program_options::variables_map& options_;

        /// The logging service of the application
        std::shared_ptr<ezserver::shared::services::ILogger> logger_;

        /// The filesystem service of the application
        std::shared_ptr<ezserver::shared::services::IFilesystem> filesystem_;
    };

    /**
     * A Lambda to get this module
     */
    auto plugins_loader_module = [] {
        return boost::di::make_injector(
            boost::di::bind<ezserver::shared::introp::IPluginsLoader>.to<PluginsLoader>()
        );
    };
}

#endif //EZSERVER_PLUGINS_LOADER_H

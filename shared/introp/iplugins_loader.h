#ifndef EZSERVER_IPLUGINS_LOADER_H
#define EZSERVER_IPLUGINS_LOADER_H

#include <introp/iplugin.h>
#include <services/iservices_manager.h>

#include <boost/dll/shared_library.hpp>

#include <memory>
#include <map>
#include <unordered_map>

namespace ezserver::shared::introp
{
    class IPluginsLoader {
    public:
        /**
         * Loads the plugins from the specified directories in the app options
         * @param plugins The plugins map into which the plugins get loaded
         */
        virtual void LoadPlugins(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins
        ) = 0;

        /**
         * Initializes a map of loaded plugins
         *
         * @param plugins           The loaded plugins map
         * @param services_manager  The services manager by which the plugins get initialized
         */
        virtual void InitializePlugins(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins,
            std::shared_ptr<ezserver::shared::services::IServicesManager>& services_manager
        ) = 0;

        /**
         * Load commands from the imported plugins
         * @param commands The map into which the commands get imported
         */
        virtual void LoadCommands(
            std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins,
            std::unordered_map<std::string, ezserver::shared::introp::ExportedCommand>& commands
        ) = 0;
    };
}

#endif //EZSERVER_IPLUGINS_LOADER_H

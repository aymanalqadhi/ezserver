#ifndef EZSERVER_IPLUGINS_LOADER_H
#define EZSERVER_IPLUGINS_LOADER_H

#include <introp/iplugin.h>
#include <boost/dll/shared_library.hpp>
#include <memory>
#include <map>

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
    };
}

#endif //EZSERVER_IPLUGINS_LOADER_H

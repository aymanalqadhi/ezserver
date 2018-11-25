#ifndef EZSERVER_IAPPLICATION_H
#define EZSERVER_IAPPLICATION_H

#include <introp/iplugin.h>
#include <net/itcp_client.h>

#include <map>

using ezserver::shared::introp::ExportedCommand;

/// Shared Namespace
namespace ezserver::shared
{
    /**
     * The main application base class
     */
    class IApplication {
    public:
        /**
         * An abstract class to startp the application
         * @return The application startup result
         */
        virtual bool Startup() = 0;

        //region Virtual Getters

        /**
         * Gets the a reference to the plugins imported by the app
         * @return The plugins imported by the app
         */
        virtual std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& Plugins() = 0;

        /**
         * Gets the imported commands from plugins by the applications
         * @return
         */
        virtual std::map<std::string, ExportedCommand>& Commands() = 0;

        /**
         * Gets the currently connected clients list
         * @return The clients list
         */
        virtual std::map<std::uint64_t, std::weak_ptr<ezserver::shared::net::ITcpClient>>& Clients() = 0;

        //endregion
    };

    /// An alias to the IApplication class
    typedef IApplication IApp;
}

#endif //EZSERVER_IAPPLICATION_H

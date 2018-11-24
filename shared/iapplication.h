#ifndef EZSERVER_IAPPLICATION_H
#define EZSERVER_IAPPLICATION_H

#include <introp/iplugin.h>
#include <net/itcp_client.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/tag.hpp>

#include <map>

using ezserver::shared::introp::ExportedCommand;

using boost::multi_index::hashed_non_unique;
using boost::multi_index::member;
using boost::multi_index::tag;

/// Shared Namespace
namespace ezserver::shared
{
    /// `commands_set` tag
    struct path{};
    struct name{};

    /// A Type definition for the command multi-indexed container.
    /// The first index is the path of the command, and the
    /// second index is the command name.
    typedef boost::multi_index_container<
    ezserver::shared::introp::ExportedCommand,
        boost::multi_index::indexed_by<
            hashed_non_unique<tag<path>, member<ExportedCommand, std::string, &ExportedCommand::path>>,
            hashed_non_unique<tag<name>, member<ExportedCommand, std::string, &ExportedCommand::name>>
        >
    > CommandsContainer;

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
        virtual CommandsContainer& Commands() = 0;

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

#ifndef EZSERVER_IPLUGIN_H
#define EZSERVER_IPLUGIN_H

#include <services/iservice.h>
#include <services/iservices_manager.h>

#include <boost/dll.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

/// Used to name the export function
#define PLUGIN_FACTORY_NAME get_plugin

#ifndef QUOTE
  #define QUOTE(str) #str
#endif
#ifndef EXPAND_AND_QUOTE
  #define EXPAND_AND_QUOTE(str) QUOTE(str)
#endif

/// Shared Namespace
namespace ezserver::shared::introp
{
    struct PluginInfo;

    /**
     * Plugin Base Class
     */
    class IPlugin {
    public:
        /**
         * Gets the plugin information
         * @return The plugin information
         */
        virtual const ezserver::shared::introp::PluginInfo Info() const noexcept = 0;

        /**
         * Gets the required services by this plugin
         * @return The required services list
         */
        virtual const std::vector<std::string> Requires() const noexcept = 0;

        /**
         * Initializes the plugin
         * @param  services The requested services map
         * @return The initialization process result
         */
        virtual bool Initialize(const std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>>& services) = 0;

        /**
         * Registers services provieded by the plugin
         * @param services_manager THe services manager service
         * @return The operation result
         */
        virtual bool RegisterServices(std::shared_ptr<ezserver::shared::services::IServicesManager>& services_manager) = 0;

        /**
         * Sets the factory variable
         * @param val
         */
        void Lib(boost::dll::shared_library&& val)
        {
            lib_ = std::move(val);
        }

    private:
        /**
         * A Factory method intialized only once
         */
        boost::dll::shared_library lib_;
    };

    /**
    * A Structure to hold the plugin information
    */
    struct PluginInfo {
    private:
        /**
         * Plugin info fields
         */
        std::string name, description, version, full_name, author;
        unsigned int priority = 0;

    public:
        /**
         * Default constructor
         * @param name          The plugin name
         * @param description   A Short description about the plugin
         * @param version       The version of the plugin
         * @param full_name     The fullname of the plugin (used internally)
         * @param author        The author of the plugin
         */
        PluginInfo(
            const std::string&& name,
            const std::string&& description,
            const std::string&& version,
            const std::string&& full_name,
            const std::string&& author,
            std::uint16_t priority
        ) {
            this->name        = std::move(name);
            this->description = std::move(description);
            this->version     = std::move(version);
            this->full_name   = std::move(full_name);
            this->author      = std::move(author);
            this->priority    = std::move(priority);
        }

        /// Gets the name of the plugin
        inline const std::string Name() const noexcept { return this->name; }

        /// Gets the description of the plugin
        inline const std::string Description() const noexcept { return this->description; }

        /// Gets the version of the plugin
        inline const std::string Version() const noexcept { return this->version; }

        /// Gets the full name of the plugin
        inline const std::string FullName() const noexcept { return this->full_name; }

        /// Gets the author name of the plugin
        inline const std::string Author() const noexcept { return this->author; }

        /**
         * The proiority level of the plugin
         * This is needed by the plugins loader
         * to determine what order should the
         * plugins be loaded in
         *
         * @return The priority level
         */
        inline const unsigned int Priority() const noexcept { return this->priority; }

        /**
         * An operator overload for the comparing operator
         * It compares the priority of each side
         * @param  val The value to compare with
         * @return True if the current priority is less than the right side priority
         */
        bool operator < (const PluginInfo &val) const
        {
            return priority < val.priority;
        }
    };
}

#endif //EZSERVER_IPLUGIN_H

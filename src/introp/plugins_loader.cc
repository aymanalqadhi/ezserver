#include "introp/plugins_loader.h"

#include <boost/dll.hpp>
#include <memory>
#include <map>
#include <vector>

void ezserver::introp::PluginsLoader::LoadPlugins(
    std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins
) {
    // Load plugins one by one
    // TODO:
    // * Do this in a some parrallel way
    for (auto &&plugin_path : filesystem_->ListDirectory(ezserver::shared::services::CommonDirectories::Plugins))
    {
        // Move to the next plugin if the current plugin_path was not
        // a valid plugin binary file
        if (boost::filesystem::is_directory(plugin_path) || !boost::filesystem::is_regular(plugin_path)) continue;

        // Try to load the plugin. If any exception is thrown,
        // just catch it, and abort the current plugin loading
        // process, and print a warning message to notifiy the
        // user.
        try
        {
            // Get an aliased factory fucntion that produces a
            // unique smart pointer to the plugin.
            // If not found, an exception is thrown
            auto plugin_factory = boost::dll::import_alias<std::unique_ptr<ezserver::shared::introp::IPlugin>()>(
                plugin_path, "get_plugin"
            );

            // Try to create the plugin from the factory function
            auto plugin = plugin_factory();

            // Check if the plugin is valid, if it is not,
            // throw a runtime error to be catched by the
            // warning exception handler
            if (!plugin)
                throw std::runtime_error("Invalid Plugin " + plugin_path.string());

            // Set the factory method
            // *[IMPORTANT]*: this line of code is what keeps
            // the plugin alive
            plugin->Factory(std::move(plugin_factory));

            // Get the information of the loaded plugin.
            // This information contains the name of the
            // plugin which is used by the program, the full
            // name of the plugin which is used as a user-friendly
            // display name, the version of the plugins, the author,
            // and the initializing priority of the plugin
            auto info = plugin->Info();
            LOG(logger_, Debug) << "Loaded " << info.Name() << ", " << info.Version()
                                << ". Written By: " << info.Author() << std::endl;

            // Finally, add the loaded plugin to the plugins contatiner
            plugins.emplace(std::make_pair(std::move(info), std::move(plugin)));
        }
        catch (const std::exception& ex)
        {
            LOG(logger_, Warning) << "Could not load plugin " << plugin_path << std::endl;;
            LOG(logger_, Trace) << ex.what() << std::endl;;
        }
    }
}

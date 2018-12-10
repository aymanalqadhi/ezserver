#include "introp/plugins_loader.h"

#include <boost/dll.hpp>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <regex>

// ========================================================= //

void ezserver::introp::PluginsLoader::LoadPlugins(
    std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins
) {
    // Load plugins one by one
    // TODO:
    // * Do this in a parrallel way
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
            // Open the shared library, and get a reference to it
            boost::dll::shared_library lib(plugin_path);

            // Get an aliased factory fucntion that produces a
            // unique smart pointer to the plugin.
            // If not found, an exception is thrown
            auto factory = lib.get_alias<std::unique_ptr<ezserver::shared::introp::IPlugin>()>(
                EXPAND_AND_QUOTE(PLUGIN_FACTORY_NAME)
            );

            // Try to create the plugin from the factory function
            auto plugin = factory();

            // Check if the plugin is valid, if it is not,
            // throw a runtime error to be catched by the
            // warning exception handler
            if (!plugin)
                throw std::runtime_error("Invalid Plugin " + plugin_path.string());

            // Set the factory method
            // *[IMPORTANT]*: this line of code is what keeps
            // the plugin alive
            plugin->Lib(std::move(lib));

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

// ========================================================= //

void ezserver::introp::PluginsLoader::InitializePlugins(
    std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>> &plugins,
    std::shared_ptr<ezserver::shared::services::IServicesManager> &services_manager
) {
    // Iterates through plugins and initialize them one by one
    // TODO:
    // * Do this in a parrallel way
    for (auto plugin_itr = plugins.begin(); plugin_itr != plugins.end(); ++plugin_itr)
    {
        try
        {
            // Try to resolve the requested dependencies
            auto deps = services_manager->ResolveDependencies(std::move(plugin_itr->second->Requires()));

            // Try to initialize the plugin, if failed, abort and remove the plugin
            if (!plugin_itr->second->Initialize(std::move(deps)))
                throw;

            // Try to import services from plugin, if failed, log a warning message
            if (!plugin_itr->second->RegisterServices(services_manager))
                LOG(logger_, Warning) << "Could not import services from plugin: "
                                      << plugin_itr->first.Name() << std::endl;
        }
        catch(const std::exception& ex)
        {
            // Remove the plugin from the plugins list
            plugins.erase(plugin_itr);

            // Log a warning & trace messages
            LOG(logger_, Warning) << "Could not initialize plugin: " << plugin_itr->first.Name()
                                  << ", so it will not be loaded!" << std::endl;
            LOG(logger_, Trace) << ex.what() << std::endl;
        }
    }
}

// ========================================================= //

void ezserver::introp::PluginsLoader::LoadCommands(
    std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>& plugins,
    std::unordered_map<std::string, ezserver::shared::introp::ExportedCommand> &commands)
{
    // Static RegularExpression object to match the commands paths and names
    static const std::regex commands_pattern(R"(^(\/[\w-\.]+)+:\w+$)");

    // Iterate through all imported plugins, and try to imported commands one by one
    // TODO:
    // Improve this to utilize multi core processors, and use
    // a multi-threaded approach
    for(auto& plugin : plugins)
    {
        try
        {
            // Iterate through all commands in the current plugin
            // TODO:
            // Improve this to utilize multi core processors, and use
            // a multi-threaded approach
            for (auto& command : plugin.second->GetCommands())
            {
                // Shape a full command specifier from the command's path
                // and name
                auto command_key = command.Path() + ":" + command.Name();

                // Try to match the command specifier with the commands validation
                // regular expression pattern. And if failed, just ignore the command,
                // and go to the next iteration (or exit if no more)
                if (!std::regex_match(command_key, commands_pattern))
                {
                    LOG(logger_, Debug) << "Ignored Invalid Command: " << command_key
                                        << " From: " << plugin.first.Name() << std::endl;
                    continue;
                }

                // Finally, add the command alogside with its specifier to the
                // commands map
                commands.emplace(std::make_pair(std::move(command_key), command));
            }
        }
        catch(const std::exception& ex)
        {
            // Print any exception message in trace level
            LOG(logger_, Trace) << ex.what() << std::endl;
        }
    }
}

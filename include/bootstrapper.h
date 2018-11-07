#ifndef EZSERVER_BOOTSTRAPPER_H
#define EZSERVER_BOOTSTRAPPER_H

#include <ibootstrapper.h>
#include <iapplication.h>
#include <services/iservices_manager.h>
#include <services/ilogger.h>
#include <services/ifilesystem.h>
#include <introp/iplugins_loader.h>

#include <boost/di.hpp>
#include <boost/dll/shared_library.hpp>
#include <memory>

/// Main Namespace
namespace ezserver
{
    /// The first thing to run in the application
    class Bootstrapper : public ezserver::shared::IBootstrapper {
    public:
        /// Base class function override
        virtual void Run() override;

        /**
         * Default dependency injection constructor
         */
        BOOST_DI_INJECT(Bootstrapper,
            const std::shared_ptr<ezserver::shared::IApplication> app,
            const std::shared_ptr<ezserver::shared::services::IServicesManager>& services_manager,
            const std::shared_ptr<ezserver::shared::services::ILogger>& logger,
            const std::shared_ptr<ezserver::shared::services::IFilesystem>& filesystem,
            const std::shared_ptr<ezserver::shared::introp::IPluginsLoader>& plugins_loader)
        : services_manager_(services_manager), application_(app),
          logger_(logger), filesystem_(filesystem), plugins_loader_(plugins_loader){

            if (!services_manager_.lock()->Initialize()) {
                throw new std::runtime_error("Could not initialize services manager!");
            }

            auto logsvc = std::shared_ptr<ezserver::shared::services::IService>(logger);
            auto fs     = std::shared_ptr<ezserver::shared::services::IService>(filesystem);

            services_manager->RegisterService(ezserver::shared::services::kLoggerService, logsvc);
            services_manager->RegisterService(ezserver::shared::services::kFilesystemService, fs);
        }

    protected:

        /// Base class method override
        virtual bool Bootstrap() override;

        /// Base class method override
        virtual bool LoadPlugins() override;

        /// Base class method override
        virtual bool LoadServices() override;

    private:
        /// A service to manage app services
        std::weak_ptr<ezserver::shared::services::IServicesManager> services_manager_;

        /// The main application object
        std::shared_ptr<ezserver::shared::IApplication> application_;

        /// A service to manage app logs
        std::weak_ptr<ezserver::shared::services::ILogger> logger_;

        /// A service to app filesystem tasks
        std::weak_ptr<ezserver::shared::services::IFilesystem> filesystem_;

        /// The plugins loader object
        std::weak_ptr<ezserver::shared::introp::IPluginsLoader> plugins_loader_;

        std::shared_ptr<std::map<ezserver::shared::introp::PluginInfo, std::unique_ptr<ezserver::shared::introp::IPlugin>>> plugins_;
    };
}

#endif //EZSERVER_BOOTSTRAPPER_H

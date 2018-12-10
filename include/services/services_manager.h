#ifndef EZSERVER_SERVICES_MANAGER_H
#define EZSERVER_SERVICES_MANAGER_H

#include "services/iservice.h"
#include "services/iservices_manager.h"

#include <boost/di.hpp>
#include <unordered_map>

/// Services namespace
namespace ezserver::services
{
    /**
     * A Service to manage application's services
     */
    class ServicesManager : public ezserver::shared::services::IServicesManager {
    public:

        /// Base class function override
        virtual bool Initialize() override;

        /// Base class method override
        virtual std::shared_ptr<ezserver::shared::services::IService> GetService(const std::string& key) override;

        /// Base class method override
        virtual std::shared_ptr<ezserver::shared::services::IService> TryGetService(const std::string& key) override;

        /// Base class method override
        virtual std::shared_ptr<ezserver::shared::services::IService> operator[](const std::string& index) override;

        /// Base class method override
        virtual void RegisterService(const std::string& key, std::shared_ptr<ezserver::shared::services::IService>& service) override;

        /// Base class method override
        virtual std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>>
            ResolveDependencies(const std::vector<std::string>& deps) override;

        /// Gets the services map
        virtual std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>> Services() override { return this->services_; }

    private:
        /**
         * The application's services container
         */
        std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>> services_;
    };

    /**
     * A Lambda to get this module
     */
    auto services_manager_module = [] {
        return boost::di::make_injector(
            boost::di::bind<ezserver::shared::services::IServicesManager>.to<ServicesManager>()
        );
    };
}

#endif //EZSERVER_SERVICES_MANAGER_H

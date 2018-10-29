#ifndef EZSERVER_ISERVICEMANAGER_H
#define EZSERVER_ISERVICEMANAGER_H

#include "services/iservice.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace ezserver::shared::services
{
    /**
    * Service unique id
    */
    const std::string kServicesManagerService = "eztech.com/ezserver/services/services-manager";

    class IServicesManager : public IService
    {
    public:

        /// Base class function override
        virtual const std::string Name() const noexcept override
        { return "ServicesManager"; }

        /// Base class function override
        virtual const bool IsRequired() const noexcept override
        { return true; }

        /**
         * Gets a service by key, if not found, an exceotion is thrown
         *
         * @param  key The service id
         * @return A Smart pointer to the service
         */
        virtual std::shared_ptr<ezserver::shared::services::IService> GetService(const std::string &key) = 0;

        /**
         * Tries to get a service by key, if not found, a nullptr is returned
         *
         * @param  key The service id
         * @return A Smart pointer to the service
         */
        virtual std::shared_ptr<ezserver::shared::services::IService> TryGetService(const std::string &key) = 0;

        /**
         * An operation overload to get a service by id, if not found, an exception is thrown
         *
         * @param  index The service unique id
         * @return A Smart pointer to the service
         */
        virtual std::shared_ptr<ezserver::shared::services::IService> operator[](const std::string &index) = 0;

        /**
        * Adds a service to the services container
        *
        * @param  key     The service id
        * @param  service A pointer to the service
        *
        * @throws std::runtime_error if the adding operation fails
        */
        virtual void
        RegisterService(const std::string &key, std::shared_ptr<ezserver::shared::services::IService> &service) = 0;

        /**
         * Gets the services map
         * @return The map of the services
         */
        virtual std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>> Services() = 0;
    };
}


#endif //EZSERVER_ISERVICEMANAGER_H

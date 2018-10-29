#ifndef EZSERVER_ISERVICES_H
#define EZSERVER_ISERVICES_H

#include <string>

/// Services namespace
namespace ezserver::shared::services
{
    /**
     * The base service class
     */
    class IService {
    public:
        /**
         * Gets the unique name of the service
         * @return The service name
         */
        virtual const std::string Name() const noexcept = 0;

        /**
         * Gets whether that the application depends of this service or not
         * @return The requiring status
         */
        virtual const bool IsRequired() const noexcept { return false; }

        /**
         * Initializes the service
         * @return The operation result
         */
        virtual bool Initialize() = 0;
    };
}

#endif //EZSERVER_ISERVICES_H

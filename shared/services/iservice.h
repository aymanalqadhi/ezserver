#ifndef EZSERVER_ISERVICES_H
#define EZSERVER_ISERVICES_H

#include <termcolor/termcolor.hpp>

#include <string>
#include <atomic>

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
         * Gets whether the service is initialized or not
         * @return
         */
        inline const bool IsBootstrapped() const noexcept { return is_bootstrapped_; }

        /**
         * Bootstraps the service
         * @return The operation result
         */
        bool Bootstrap()
        {
            // Return true if the service is already bootstrapped
            if (IsBootstrapped()) return true;

            // Initialize the service
            if (!Initialize()) return false;
            SetBootstrapped();

            return true;
        }

    protected:

        /**
         * Default constructor
         */
        IService() : is_bootstrapped_(false) {}

        /**
         * Initializes the service
         * @return The operation result
         */
        virtual bool Initialize() = 0;

        /**
         * Sets the service as initialized
         */
        inline void SetBootstrapped() noexcept { is_bootstrapped_.store(true); }

    private:

        /**
         * The atomic holder of the initialization status variable
         */
        std::atomic_bool is_bootstrapped_;
    };
}

#endif //EZSERVER_ISERVICES_H

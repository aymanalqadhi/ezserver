#ifndef EZSERVER_IAPPLICATION_H
#define EZSERVER_IAPPLICATION_H

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
    };

    /// An alias to the IApplication class
    typedef IApplication IApp;
}

#endif //EZSERVER_IAPPLICATION_H

#ifndef EZSERVER_IBOOTSTRAPPER_H
#define EZSERVER_IBOOTSTRAPPER_H

/// Shared Namespace
namespace ezserver::shared
{
    /**
     * The first class to run in the application
     * It initializes the application's services
     * and then startups the application
     */
    class IBootstrapper
    {
    protected:
        /**
         * Bootstraps application's serviceses
         */
        virtual void Bootstrap() = 0;

        /**
         * Loads plugins in the plugins directory
         * @return The operation result
         */
        virtual bool LoadPlugins() = 0;

        /**
         * Loads the services provided by the app
         * @return The operation result
         */
        virtual bool LoadServices() = 0;

    public:
        /**
         * Runs the application
         * @return The operation result
         */
        virtual void Run() = 0;
    };
}

#endif //EZSERVER_IBOOTSTRAPPER_H

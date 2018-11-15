#ifndef EZSERVER_EVENT_H
#define EZSERVER_EVENT_H

#include <functional>
#include <vector>

namespace ezserver::shared::utils
{
    /**
     * A Generic event handler like the one used in C#
     * @tparam TSender  The sender type
     * @tparam TPayload The payload type
     */
    template<typename TSender, typename TPayload>
    class EventHandler
    {

    public:
        /**
         * Registers a handler to this event
         * @param callback The handler to register
         */
        virtual void RegisterHandler(const std::function<void(TSender, TPayload)>& callback) {
            callbacks_.push_back(callback);
        }

        /**
         * Registers a handler in C# way
         * @param handler The handler to register
         */
        virtual void operator+=(const std::function<void(TSender, TPayload)>& handler) {
            RegisterHandler(handler);
        }

        /**
         * Invokes all registerd handlers
         * @param payload The data to post on all handlers
         */
        virtual void Invoke(TSender sender, TPayload payload)
        {
            for (const auto& cb : callbacks_)
                cb(sender, payload);
        }

    private:
        /// The handlers container
        std::vector<std::function<void(TSender, TPayload)>> callbacks_;
    };
}

#endif //EZSERVER_EVENT_H

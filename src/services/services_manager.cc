#include <services/services_manager.h>

#include <stdexcept>
#include <string>

// ============================================================== //

bool ezserver::services::ServicesManager::Initialize()
{
    // Just return true since there is
    // no real initialization
    return true;
}

// ============================================================== //

void ezserver::services::ServicesManager::RegisterService(const std::string &key, std::shared_ptr<IService> &service)
{
    // Check the service for a null value.
    // If the value was null, then throw an exception.
    // If the service is already registerd, throw an exception
    if (service == nullptr)
        throw std::runtime_error("Service cannot be null!");
    else if (this->services_.find(key) != this->services_.end())
        throw std::runtime_error("Service is already registered!!");

    // Register the service with the gived key
    this->services_[key] = service;
}

// ============================================================== //

std::shared_ptr<ezserver::shared::services::IService>
ezserver::services::ServicesManager::GetService(const std::string &key)
{
    // Check if thw service is not registerd, if so, throw an exceotion
    if (this->services_.find(key) == this->services_.end())
        throw std::out_of_range("Service is not registered!!");

    // Return the service with the given key
    return this->services_[key];
}

// ============================================================== //

std::shared_ptr<ezserver::shared::services::IService>
ezserver::services::ServicesManager::TryGetService(const std::string &key)
{
    // Try to find a service, if not found, just return a nullptr
    try
    {
        auto ret = this->GetService(key);
        return ret;
    }
    catch (std::out_of_range)
    {
        return nullptr;
    }
}

// ============================================================== //

std::shared_ptr<ezserver::shared::services::IService>
ezserver::services::ServicesManager::operator[](const std::string &index)
{
    // Return service with the given key
    return this->GetService(index);
}

// ============================================================== //

std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>>
ezserver::services::ServicesManager::ResolveDependencies(const std::vector<std::string>& deps)
{
    // The map that will be returned
    std::unordered_map<std::string, std::shared_ptr<ezserver::shared::services::IService>> dependencies;

    // Iterate throguh the dependencies vector, and resolve
    // all requested services
    for (const auto& dep : deps)
    {
        // Try to resolve a service
        auto svc = TryGetService(dep);

        // If the resolving operation was failed, throw an exception
        if (svc == nullptr)
            throw std::runtime_error("Cannot resolve service: " + dep);

        // Add the resolved dependency to the dependencies map
        dependencies.emplace(std::make_pair(dep, std::move(svc)));
    }

    // Return the resolved dependencies map
    return dependencies;
}
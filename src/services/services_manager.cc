#include <services/services_manager.h>

#include <stdexcept>
#include <string>

bool ezserver::services::ServicesManager::Initialize() {
    return true;
}

void ezserver::services::ServicesManager::RegisterService(const std::string &key, std::shared_ptr<IService> &service) {
    if (service == nullptr) {
        throw std::runtime_error("Service cannot be null!");
    } else if (this->services_.find(key) != this->services_.end()) {
        throw std::runtime_error("Service is already registered!!");
    }
    this->services_[key] = service;
}

std::shared_ptr<ezserver::shared::services::IService> ezserver::services::ServicesManager::GetService(const std::string &key) {
    if (this->services_.find(key) == this->services_.end()) {
        throw std::out_of_range("Service is not registered!!");
    }

    return this->services_[key];
}

std::shared_ptr<ezserver::shared::services::IService> ezserver::services::ServicesManager::TryGetService(const std::string &key) {
    try {
        auto ret = this->GetService(key);
        return ret;
    } catch (std::out_of_range) {
        return nullptr;
    }
}

std::shared_ptr<ezserver::shared::services::IService> ezserver::services::ServicesManager::operator[](const std::string &index) {
    return this->GetService(index);
}
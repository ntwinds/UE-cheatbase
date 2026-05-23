#pragma once

#include "core/Types.hpp"
#include <functional>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <typeindex>

namespace cheatbase::runtime {

class IEvent {
public:
    virtual ~IEvent() = default;
};

class EventBus {
public:
    using EventHandler = std::function<void(const IEvent&)>;

    template<typename T>
    void Subscribe(std::function<void(const T&)> callback) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Subscribers[typeid(T)].push_back([callback](const IEvent& e) {
            callback(static_cast<const T&>(e));
        });
    }

    template<typename T>
    void Publish(const T& event) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_Subscribers.find(typeid(T));
        if (it != m_Subscribers.end()) {
            for (auto& handler : it->second) {
                handler(event);
            }
        }
    }

private:
    std::mutex m_Mutex;
    std::unordered_map<std::type_index, std::vector<EventHandler>> m_Subscribers;
};

} // namespace cheatbase::runtime

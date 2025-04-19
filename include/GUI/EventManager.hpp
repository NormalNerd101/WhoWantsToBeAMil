// EventManager.hpp
#include <functional>
#include <map>
#include <string>
#include <vector>

class EventManager {
private:
    std::map<std::string, std::vector<std::function<void()>>> callbacks;
    
    // Singleton implementation
    static EventManager* instance;
    EventManager() {}

public:
    static EventManager& getInstance() {
        if (!instance) {
            instance = new EventManager();
        }
        return *instance;
    }
    
    // Register a callback for an event
    void registerCallback(const std::string& eventName, std::function<void()> callback) {
        callbacks[eventName].push_back(callback);
    }
    
    // Trigger an event
    void triggerEvent(const std::string& eventName) {
        if (callbacks.find(eventName) != callbacks.end()) {
            for (auto& callback : callbacks[eventName]) {
                callback();
            }
        }
    }
};

// Initialize the static instance
EventManager* EventManager::instance = nullptr;
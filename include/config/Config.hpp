#pragma once

#include "core/Types.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include "core/Logger.hpp"

namespace cheatbase::config {

class ConfigManager {
public:
    static ConfigManager& Get() {
        static ConfigManager instance;
        return instance;
    }

    bool Load(std::string_view filepath) {
        std::ifstream file(filepath.data());
        if (!file.is_open()) return false;
        
        try {
            m_Data = nlohmann::json::parse(file);
            return true;
        } catch (const nlohmann::json::exception& e) {
            CHEATBASE_ERROR("ConfigManager: JSON parse error: {}", e.what());
            return false;
        }
    }

    bool Save(std::string_view filepath) {
        std::ofstream file(filepath.data());
        if (!file.is_open()) return false;
        
        file << m_Data.dump(4);
        return true;
    }

    template<typename T>
    T GetValue(const std::string& key, const T& defaultValue) {
        if (m_Data.contains(key)) {
            return m_Data[key].get<T>();
        }
        return defaultValue;
    }

    template<typename T>
    void SetValue(const std::string& key, const T& value) {
        m_Data[key] = value;
    }

private:
    nlohmann::json m_Data;
};

} // namespace cheatbase::config

#pragma once

#include "process/Process.hpp"
#include <string>

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace cheatbase::process {

class MemoryReader {
public:
    explicit MemoryReader(const Process& process) : m_Process(process) {}

    template<typename T>
    CHEATBASE_NODISCARD T Read(core::Address address) const {
        T buffer{};
        ReadRaw(address, &buffer, sizeof(T));
        return buffer;
    }

    template<typename T>
    bool Write(core::Address address, const T& value) const {
        return WriteRaw(address, &value, sizeof(T));
    }

    CHEATBASE_NODISCARD std::string ReadString(core::Address address, size_t maxLength = 256) const {
        std::string result(maxLength, '\0');
        
        if (!m_Process.IsValid() || !m_Process.GetBackend() || !address) return "";

        m_Process.GetBackend()->ReadRaw(address, result.data(), maxLength);
        
        result.resize(strnlen(result.data(), maxLength));
        return result;
    }

private:
    const Process& m_Process;

    bool ReadRaw(core::Address address, void* buffer, size_t size) const {
        if (!m_Process.IsValid() || !m_Process.GetBackend() || !address) return false;
        return m_Process.GetBackend()->ReadRaw(address, buffer, size);
    }

    bool WriteRaw(core::Address address, const void* buffer, size_t size) const {
        if (!m_Process.IsValid() || !m_Process.GetBackend() || !address) return false;
        return m_Process.GetBackend()->WriteRaw(address, buffer, size);
    }
};

} // namespace cheatbase::process

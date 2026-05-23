#pragma once

#include "core/Types.hpp"
#include "process/IMemoryBackend.hpp"
#include <string>
#include <vector>

namespace cheatbase::process {

struct ModuleInfo {
    std::string Name;
    core::Address BaseAddress{0};
    core::u32 Size{0};
};

class Process {
public:
    Process();
    ~Process();

    Process(const Process&) = delete;
    Process& operator=(const Process&) = delete;

    bool Attach(std::string_view processName, IMemoryBackend* backend);
    void Detach();

    CHEATBASE_NODISCARD bool IsValid() const { return m_ProcessId != 0; }
    CHEATBASE_NODISCARD core::u32 GetProcessId() const { return m_ProcessId; }
    
    CHEATBASE_NODISCARD std::optional<ModuleInfo> GetModule(std::string_view moduleName) const;
    CHEATBASE_NODISCARD core::Address GetBaseAddress() const;

    IMemoryBackend* GetBackend() const { return m_Backend; }

private:
    core::u32 m_ProcessId{0};
    IMemoryBackend* m_Backend{nullptr};
    std::vector<ModuleInfo> m_Modules;
    
    void CacheModules();
};

} // namespace cheatbase::process

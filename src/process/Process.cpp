#include "process/Process.hpp"
#include "core/Logger.hpp"
#include "platform/WindowsPlatform.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#include <TlHelp32.h>
#endif

namespace cheatbase::process {

Process::Process() = default;

Process::~Process() {
    Detach();
}

bool Process::Attach(std::string_view processName, IMemoryBackend* backend) {
    if (IsValid()) Detach();
    
    m_Backend = backend;
    if (!m_Backend) {
        CHEATBASE_ERROR("Process: No memory backend provided.");
        return false;
    }

#ifdef CHEATBASE_PLATFORM_WINDOWS
    std::wstring wProcessName = platform::WindowsPlatform::Utf8ToWide(processName);
    
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        CHEATBASE_ERROR("Failed to create process snapshot: {}", platform::WindowsPlatform::GetLastErrorMessage());
        return false;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (Process32FirstW(snapshot, &processEntry)) {
        do {
            if (std::wstring_view(processEntry.szExeFile) == wProcessName) {
                m_ProcessId = processEntry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &processEntry));
    }
    CloseHandle(snapshot);

    if (m_ProcessId == 0) {
        CHEATBASE_WARN("Process {} not found.", processName);
        return false;
    }

    // Use the abstract backend instead of OpenProcess
    if (!m_Backend->AttachToProcess(m_ProcessId)) {
        CHEATBASE_ERROR("Memory Backend failed to attach to PID: {}", m_ProcessId);
        return false;
    }

    CacheModules();
    CHEATBASE_INFO("Attached to process {} (PID: {}) via Backend.", processName, m_ProcessId);
    return true;
#else
    return false;
#endif
}

void Process::Detach() {
    m_ProcessId = 0;
    m_Modules.clear();
    m_Backend = nullptr;
}

std::optional<ModuleInfo> Process::GetModule(std::string_view moduleName) const {
    for (const auto& mod : m_Modules) {
        if (mod.Name == moduleName) {
            return mod;
        }
    }
    return std::nullopt;
}

core::Address Process::GetBaseAddress() const {
    if (m_Modules.empty()) return 0;
    return m_Modules.front().BaseAddress; // Usually the first module is the main exe
}

void Process::CacheModules() {
    m_Modules.clear();
#ifdef CHEATBASE_PLATFORM_WINDOWS
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, m_ProcessId);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    MODULEENTRY32W moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);

    if (Module32FirstW(snapshot, &moduleEntry)) {
        do {
            ModuleInfo info;
            info.Name = platform::WindowsPlatform::WideToUtf8(moduleEntry.szModule);
            info.BaseAddress = reinterpret_cast<core::Address>(moduleEntry.modBaseAddr);
            info.Size = moduleEntry.modBaseSize;
            m_Modules.push_back(info);
        } while (Module32NextW(snapshot, &moduleEntry));
    }
    CloseHandle(snapshot);
#endif
}

} // namespace cheatbase::process

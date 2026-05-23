#include "process/UsermodeBackend.hpp"
#include "core/Logger.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace cheatbase::process {

UsermodeBackend::UsermodeBackend() = default;

UsermodeBackend::~UsermodeBackend() {
    Shutdown();
}

bool UsermodeBackend::Initialize() {
    return true; // No special init needed for RPM
}

void UsermodeBackend::Shutdown() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (m_ProcessHandle) {
        CloseHandle(m_ProcessHandle);
        m_ProcessHandle = nullptr;
    }
#endif
}

bool UsermodeBackend::AttachToProcess(core::u32 processId) {
    Shutdown(); // Clear existing
#ifdef CHEATBASE_PLATFORM_WINDOWS
    m_ProcessHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    return m_ProcessHandle != nullptr;
#else
    return false;
#endif
}

bool UsermodeBackend::ReadRaw(core::Address address, void* buffer, size_t size) const {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (!m_ProcessHandle) return false;
    SIZE_T bytesRead = 0;
    return ReadProcessMemory(m_ProcessHandle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead) && bytesRead == size;
#else
    return false;
#endif
}

bool UsermodeBackend::WriteRaw(core::Address address, const void* buffer, size_t size) const {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (!m_ProcessHandle) return false;
    SIZE_T bytesWritten = 0;
    return WriteProcessMemory(m_ProcessHandle, reinterpret_cast<LPVOID>(address), buffer, size, &bytesWritten) && bytesWritten == size;
#else
    return false;
#endif
}

} // namespace cheatbase::process

#include "process/DriverBackend.hpp"
#include "core/Logger.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace cheatbase::process {

DriverBackend::DriverBackend(std::string_view registryPath) 
    : m_RegistryPath(registryPath) {}

DriverBackend::~DriverBackend() {
    Shutdown();
}

bool DriverBackend::Initialize() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    m_DriverHandle = CreateFileA(m_RegistryPath.c_str(), GENERIC_READ | GENERIC_WRITE, 
                                 FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    
    if (m_DriverHandle == INVALID_HANDLE_VALUE) {
        CHEATBASE_ERROR("DriverBackend: Failed to open handle to driver at {}", m_RegistryPath);
        m_DriverHandle = nullptr;
        return false;
    }
    return true;
#else
    return false;
#endif
}

void DriverBackend::Shutdown() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (m_DriverHandle) {
        CloseHandle(m_DriverHandle);
        m_DriverHandle = nullptr;
    }
#endif
}

bool DriverBackend::AttachToProcess(core::u32 processId) {
    m_TargetProcessId = processId;
    return m_DriverHandle != nullptr;
}

bool DriverBackend::ReadRaw(core::Address address, void* buffer, size_t size) const {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (!m_DriverHandle || !m_TargetProcessId) return false;

    DriverRequest req{};
    req.ProcessId = m_TargetProcessId;
    req.Address = address;
    req.Buffer = buffer;
    req.Size = size;

    DWORD bytesReturned = 0;
    return DeviceIoControl(m_DriverHandle, IOCTL_READ_MEMORY, &req, sizeof(req), &req, sizeof(req), &bytesReturned, nullptr);
#else
    return false;
#endif
}

bool DriverBackend::WriteRaw(core::Address address, const void* buffer, size_t size) const {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (!m_DriverHandle || !m_TargetProcessId) return false;

    DriverRequest req{};
    req.ProcessId = m_TargetProcessId;
    req.Address = address;
    req.Buffer = const_cast<void*>(buffer); 
    req.Size = size;

    DWORD bytesReturned = 0;
    return DeviceIoControl(m_DriverHandle, IOCTL_WRITE_MEMORY, &req, sizeof(req), &req, sizeof(req), &bytesReturned, nullptr);
#else
    return false;
#endif
}

} // namespace cheatbase::process

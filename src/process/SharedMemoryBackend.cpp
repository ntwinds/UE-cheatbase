#include "process/SharedMemoryBackend.hpp"
#include "core/Logger.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace cheatbase::process {

SharedMemoryBackend::SharedMemoryBackend() = default;

SharedMemoryBackend::~SharedMemoryBackend() {
    Shutdown();
}

bool SharedMemoryBackend::Initialize() {
    CHEATBASE_INFO("SharedMemoryBackend: Initializing shared memory struct...");
    return true; 
}

void SharedMemoryBackend::Shutdown() {
}

bool SharedMemoryBackend::AttachToProcess(core::u32 processId) {
    m_TargetProcessId = processId;
    return true;
}

bool SharedMemoryBackend::ReadRaw(core::Address address, void* buffer, size_t size) const {
    return false;
}

bool SharedMemoryBackend::WriteRaw(core::Address address, const void* buffer, size_t size) const {
    return false;
}

} // namespace cheatbase::process

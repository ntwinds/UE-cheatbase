#pragma once

#include "process/IMemoryBackend.hpp"
#include <string>

namespace cheatbase::process {

struct DriverRequest {
    core::u32 ProcessId;
    core::Address Address;
    void* Buffer;
    size_t Size;
};

constexpr core::u32 IOCTL_READ_MEMORY = 0x12345678; 
constexpr core::u32 IOCTL_WRITE_MEMORY = 0x12345679;

class DriverBackend : public IMemoryBackend {
public:
    explicit DriverBackend(std::string_view registryPath = "\\\\.\\YourDriverName");
    ~DriverBackend() override;

    bool Initialize() override;
    void Shutdown() override;

    bool AttachToProcess(core::u32 processId) override;

    bool ReadRaw(core::Address address, void* buffer, size_t size) const override;
    bool WriteRaw(core::Address address, const void* buffer, size_t size) const override;

private:
    std::string m_RegistryPath;
    void* m_DriverHandle{nullptr};
    core::u32 m_TargetProcessId{0};
};

} // namespace cheatbase::process

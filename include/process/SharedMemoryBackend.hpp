#pragma once

#include "process/IMemoryBackend.hpp"
#include <string>

namespace cheatbase::process {

class SharedMemoryBackend : public IMemoryBackend {
public:
    SharedMemoryBackend();
    ~SharedMemoryBackend() override;

    bool Initialize() override;
    void Shutdown() override;

    bool AttachToProcess(core::u32 processId) override;

    bool ReadRaw(core::Address address, void* buffer, size_t size) const override;
    bool WriteRaw(core::Address address, const void* buffer, size_t size) const override;

private:
    void* m_SharedMemoryView{nullptr};
    void* m_MappingHandle{nullptr};
    core::u32 m_TargetProcessId{0};
};

} // namespace cheatbase::process

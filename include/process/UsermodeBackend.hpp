#pragma once

#include "process/IMemoryBackend.hpp"

namespace cheatbase::process {

class UsermodeBackend : public IMemoryBackend {
public:
    UsermodeBackend();
    ~UsermodeBackend() override;

    bool Initialize() override;
    void Shutdown() override;

    bool AttachToProcess(core::u32 processId) override;

    bool ReadRaw(core::Address address, void* buffer, size_t size) const override;
    bool WriteRaw(core::Address address, const void* buffer, size_t size) const override;

private:
    void* m_ProcessHandle{nullptr};
};

} // namespace cheatbase::process

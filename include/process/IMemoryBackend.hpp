#pragma once

#include "core/Types.hpp"

namespace cheatbase::process {

class IMemoryBackend {
public:
    virtual ~IMemoryBackend() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual bool AttachToProcess(core::u32 processId) = 0;

    virtual bool ReadRaw(core::Address address, void* buffer, size_t size) const = 0;
    virtual bool WriteRaw(core::Address address, const void* buffer, size_t size) const = 0;
};

} // namespace cheatbase::process

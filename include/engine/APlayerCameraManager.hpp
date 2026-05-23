#pragma once

#include "engine/UObject.hpp"
#include "engine/EngineOffsets.hpp"
#include "core/Math.hpp"

namespace cheatbase::engine {

struct FCameraCacheEntry {
    float Timestamp;
    char pad[0xC];
    core::Vector3 Location;
    core::Vector3 Rotation;
    float FOV;
};

class APlayerCameraManager : public UObject {
public:
    explicit APlayerCameraManager(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD FCameraCacheEntry GetCameraCache(const process::MemoryReader& memory) const {
        return memory.Read<FCameraCacheEntry>(GetAddress() + Offsets::Get().APlayerCameraManager_CameraCachePrivate);
    }
};

} // namespace cheatbase::engine

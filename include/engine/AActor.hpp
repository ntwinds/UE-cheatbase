#pragma once

#include "engine/UObject.hpp"
#include "engine/EngineOffsets.hpp"
#include "core/Math.hpp"

namespace cheatbase::engine {

class USceneComponent : public UObject {
public:
    explicit USceneComponent(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD core::Vector3 GetRelativeLocation(const process::MemoryReader& memory) const {
        return memory.Read<core::Vector3>(GetAddress() + Offsets::Get().USceneComponent_RelativeLocation);
    }
    
    // Some games use ComponentToWorld for absolute location
    CHEATBASE_NODISCARD FTransform GetComponentToWorld(const process::MemoryReader& memory) const {
        return memory.Read<FTransform>(GetAddress() + Offsets::Get().USceneComponent_ComponentToWorld);
    }
};

class AActor : public UObject {
public:
    explicit AActor(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD USceneComponent GetRootComponent(const process::MemoryReader& memory) const {
        return USceneComponent(memory.Read<core::Address>(GetAddress() + Offsets::Get().AActor_RootComponent));
    }
};

} // namespace cheatbase::engine

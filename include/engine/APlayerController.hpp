#pragma once

#include "engine/UObject.hpp"
#include "engine/APlayerCameraManager.hpp"
#include "engine/EngineOffsets.hpp"

namespace cheatbase::engine {

class APlayerController : public UObject {
public:
    explicit APlayerController(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD UObject GetPawn(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(GetAddress() + Offsets::Get().APlayerController_Pawn));
    }

    CHEATBASE_NODISCARD APlayerCameraManager GetCameraManager(const process::MemoryReader& memory) const {
        return APlayerCameraManager(memory.Read<core::Address>(GetAddress() + Offsets::Get().APlayerController_PlayerCameraManager));
    }
};

} // namespace cheatbase::engine

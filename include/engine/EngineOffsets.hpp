#pragma once

#include "core/Types.hpp"

namespace cheatbase::engine {

struct EngineOffsets {
    core::u32 FNamePool = 0x0;
    core::u32 GObjects = 0x0;
    core::u32 GWorld = 0x0;
    
    // UObject
    core::u32 UObject_NamePrivate = 0x18;
    core::u32 UObject_ClassPrivate = 0x10;
    core::u32 UObject_OuterPrivate = 0x20;

    // UWorld
    core::u32 UWorld_OwningGameInstance = 0x190;
    core::u32 UWorld_PersistentLevel = 0x30;

    // ULevel
    core::u32 ULevel_Actors = 0x98;

    // UGameInstance
    core::u32 UGameInstance_LocalPlayers = 0x38;

    // UPlayer
    core::u32 UPlayer_PlayerController = 0x30;

    // APlayerController
    core::u32 APlayerController_Pawn = 0x2A8;
    core::u32 APlayerController_PlayerCameraManager = 0x2C0;

    // APlayerCameraManager
    core::u32 APlayerCameraManager_CameraCachePrivate = 0x1AE0;

    // AActor
    core::u32 AActor_RootComponent = 0x138;

    // USceneComponent
    core::u32 USceneComponent_RelativeLocation = 0x11C;
    core::u32 USceneComponent_ComponentToWorld = 0x1C0;

    // USkeletalMeshComponent
    core::u32 USkeletalMeshComponent_BoneArray = 0x4B0;
    core::u32 USkeletalMeshComponent_CachedBoneSpaceTransforms = 0x4C0;
};

// Singleton to hold current active offsets
class Offsets {
public:
    static EngineOffsets& Get() {
        static EngineOffsets instance;
        return instance;
    }
};

} // namespace cheatbase::engine

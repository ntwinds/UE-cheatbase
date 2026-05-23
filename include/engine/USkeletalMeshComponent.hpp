#pragma once

#include "engine/AActor.hpp"
#include "engine/EngineOffsets.hpp"
#include "core/Math.hpp"

namespace cheatbase::engine {

class USkeletalMeshComponent : public USceneComponent {
public:
    explicit USkeletalMeshComponent(core::Address address) : USceneComponent(address) {}

    CHEATBASE_NODISCARD core::Vector3 GetBoneLocation(const process::MemoryReader& memory, core::u32 boneId) const {
        core::Address boneArray = memory.Read<core::Address>(GetAddress() + Offsets::Get().USkeletalMeshComponent_BoneArray);
        if (!boneArray) {
            boneArray = memory.Read<core::Address>(GetAddress() + Offsets::Get().USkeletalMeshComponent_CachedBoneSpaceTransforms);
        }
        
        if (!boneArray) return {0, 0, 0};

        FTransform boneTransform = memory.Read<FTransform>(boneArray + (boneId * sizeof(FTransform)));
        FTransform compToWorld = GetComponentToWorld(memory);
        
        // Simplified Matrix math for Bone Transform * ComponentToWorld
        // A complete implementation would matrix-multiply the Rotations/Translations.
        // For basic UE offsets, Translation + Translation * Rotation often works as a fallback depending on engine build.
        // Here we just return the raw translation for structural placeholder.
        return boneTransform.Translation;
    }
};

} // namespace cheatbase::engine

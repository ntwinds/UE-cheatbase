#pragma once

#include "engine/UObject.hpp"
#include <vector>

namespace cheatbase::engine {

class UWorld : public UObject {
public:
    explicit UWorld(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD UObject GetPersistentLevel(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(GetAddress() + Offsets::Get().UWorld_PersistentLevel));
    }

    CHEATBASE_NODISCARD UObject GetOwningGameInstance(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(GetAddress() + Offsets::Get().UWorld_OwningGameInstance));
    }

    // High level helper to get actors
    CHEATBASE_NODISCARD std::vector<UObject> GetActors(const process::MemoryReader& memory) const {
        std::vector<UObject> actors;
        UObject persistentLevel = GetPersistentLevel(memory);
        if (!persistentLevel.IsValid()) return actors;

        core::Address actorArray = memory.Read<core::Address>(persistentLevel.GetAddress() + Offsets::Get().ULevel_Actors);
        core::u32 actorCount = memory.Read<core::u32>(persistentLevel.GetAddress() + Offsets::Get().ULevel_Actors + 0x8);

        if (actorCount > 100000) return actors; // Sanity check

        std::vector<core::Address> actorAddresses(actorCount);
        // Note: Direct raw read for efficiency
        // A complete implementation would use a chunked read
        for (core::u32 i = 0; i < actorCount; ++i) {
            core::Address actorAddr = memory.Read<core::Address>(actorArray + (i * sizeof(core::Address)));
            if (actorAddr) {
                actors.emplace_back(actorAddr);
            }
        }

        return actors;
    }
};

} // namespace cheatbase::engine

#pragma once

#include "engine/UObject.hpp"
#include "engine/EngineOffsets.hpp"
#include <vector>

namespace cheatbase::engine {

class ULocalPlayer : public UObject {
public:
    explicit ULocalPlayer(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD UObject GetPlayerController(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(GetAddress() + Offsets::Get().UPlayer_PlayerController));
    }
};

class UGameInstance : public UObject {
public:
    explicit UGameInstance(core::Address address) : UObject(address) {}

    CHEATBASE_NODISCARD std::vector<ULocalPlayer> GetLocalPlayers(const process::MemoryReader& memory) const {
        std::vector<ULocalPlayer> players;
        core::Address localPlayersArray = memory.Read<core::Address>(GetAddress() + Offsets::Get().UGameInstance_LocalPlayers);
        core::u32 localPlayersCount = memory.Read<core::u32>(GetAddress() + Offsets::Get().UGameInstance_LocalPlayers + 0x8);

        if (localPlayersCount > 10) return players;

        for (core::u32 i = 0; i < localPlayersCount; ++i) {
            core::Address playerAddr = memory.Read<core::Address>(localPlayersArray + (i * sizeof(core::Address)));
            if (playerAddr) {
                players.emplace_back(playerAddr);
            }
        }
        return players;
    }
};

} // namespace cheatbase::engine

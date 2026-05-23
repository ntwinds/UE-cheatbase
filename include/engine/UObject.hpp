#pragma once

#include "core/Types.hpp"
#include "process/MemoryReader.hpp"
#include "engine/FName.hpp"
#include "engine/EngineOffsets.hpp"
#include <string>

namespace cheatbase::engine {

class UObject {
public:
    explicit UObject(core::Address address) : m_Address(address) {}

    CHEATBASE_NODISCARD bool IsValid() const { return m_Address != 0; }
    CHEATBASE_NODISCARD core::Address GetAddress() const { return m_Address; }

    CHEATBASE_NODISCARD core::u32 GetNameIndex(const process::MemoryReader& memory) const {
        return memory.Read<core::u32>(m_Address + Offsets::Get().UObject_NamePrivate);
    }

    CHEATBASE_NODISCARD std::string GetName(const process::MemoryReader& memory, core::Address namePool) const {
        core::u32 index = GetNameIndex(memory);
        FName name(index);
        return name.ToString(memory, namePool);
    }

    CHEATBASE_NODISCARD UObject GetClass(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(m_Address + Offsets::Get().UObject_ClassPrivate));
    }

    CHEATBASE_NODISCARD UObject GetOuter(const process::MemoryReader& memory) const {
        return UObject(memory.Read<core::Address>(m_Address + Offsets::Get().UObject_OuterPrivate));
    }

private:
    core::Address m_Address;
};

} // namespace cheatbase::engine

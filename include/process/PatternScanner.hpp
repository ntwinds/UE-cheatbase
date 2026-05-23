#pragma once

#include "process/Process.hpp"
#include "process/MemoryReader.hpp"
#include <string>
#include <vector>
#include <optional>

namespace cheatbase::process {

class PatternScanner {
public:
    PatternScanner(const Process& process, const MemoryReader& memory);

    // Signature format: "48 8B 05 ? ? ? ? 48 85 C0 74 ? 48 8B 48 08"
    CHEATBASE_NODISCARD std::optional<core::Address> FindPattern(std::string_view moduleName, std::string_view signature) const;
    
    // Helper for resolving RIP-relative addresses (common in x64)
    CHEATBASE_NODISCARD std::optional<core::Address> ResolveRelativeAddress(core::Address instructionAddress, core::u32 offset, core::u32 instructionSize) const;

private:
    const Process& m_Process;
    const MemoryReader& m_Memory;

    struct PatternByte {
        core::u8 Value;
        bool IsWildcard;
    };

    CHEATBASE_NODISCARD std::vector<PatternByte> ParseSignature(std::string_view signature) const;
};

} // namespace cheatbase::process

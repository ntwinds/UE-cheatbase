#include "process/PatternScanner.hpp"
#include "core/Logger.hpp"
#include <sstream>

namespace cheatbase::process {

PatternScanner::PatternScanner(const Process& process, const MemoryReader& memory)
    : m_Process(process), m_Memory(memory) {}

std::vector<PatternScanner::PatternByte> PatternScanner::ParseSignature(std::string_view signature) const {
    std::vector<PatternByte> pattern;
    std::istringstream iss{std::string(signature)};
    std::string token;

    while (iss >> token) {
        if (token == "?" || token == "??") {
            pattern.push_back({0, true});
        } else {
            pattern.push_back({static_cast<core::u8>(std::stoul(token, nullptr, 16)), false});
        }
    }
    return pattern;
}

std::optional<core::Address> PatternScanner::FindPattern(std::string_view moduleName, std::string_view signature) const {
    auto mod = m_Process.GetModule(moduleName);
    if (!mod) {
        CHEATBASE_WARN("PatternScanner: Module {} not found.", moduleName);
        return std::nullopt;
    }

    auto pattern = ParseSignature(signature);
    if (pattern.empty()) return std::nullopt;

    // Read the entire module into memory for fast scanning
    // Note: For huge modules, chunking is preferred. This is a simplified approach for demonstration.
    std::vector<core::u8> localBuffer(mod->Size);
    if (!m_Process.GetBackend() || !m_Process.GetBackend()->ReadRaw(mod->BaseAddress, localBuffer.data(), mod->Size)) {
        CHEATBASE_ERROR("PatternScanner: Failed to read module memory for {}", moduleName);
        return std::nullopt;
    }

    const size_t patternSize = pattern.size();
    for (size_t i = 0; i < mod->Size - patternSize; ++i) {
        bool found = true;
        for (size_t j = 0; j < patternSize; ++j) {
            if (pattern[j].IsWildcard) continue;
            if (localBuffer[i + j] != pattern[j].Value) {
                found = false;
                break;
            }
        }
        if (found) {
            return mod->BaseAddress + i;
        }
    }

    return std::nullopt;
}

std::optional<core::Address> PatternScanner::ResolveRelativeAddress(core::Address instructionAddress, core::u32 offset, core::u32 instructionSize) const {
    if (!instructionAddress) return std::nullopt;

    core::i32 relativeOffset = m_Memory.Read<core::i32>(instructionAddress + offset);
    if (!relativeOffset) return std::nullopt;

    return instructionAddress + instructionSize + relativeOffset;
}

} // namespace cheatbase::process

#include "engine/FName.hpp"
#include "core/Logger.hpp"

namespace cheatbase::engine {

std::string FName::ToString(const process::MemoryReader& memory, core::Address namePoolAddress) const {
    if (!namePoolAddress) return "None";

    core::u32 block = ComparisonIndex >> 16;
    core::u16 offset = ComparisonIndex & 65535;

    // UE5 NamePool logic (simplified for demonstration)
    core::Address namePoolChunk = memory.Read<core::Address>(namePoolAddress + 0x10 + (block * 8));
    if (!namePoolChunk) return "None";

    core::Address entryHeader = namePoolChunk + (offset * 2); // FNameEntry header size varies
    
    core::u16 header = memory.Read<core::u16>(entryHeader);
    core::u16 length = header >> 6;
    
    if (length == 0 || length > 1024) return "None";
    
    std::string name = memory.ReadString(entryHeader + 2, length);
    
    if (Number > 0) {
        name += "_" + std::to_string(Number - 1);
    }
    
    return name;
}

} // namespace cheatbase::engine

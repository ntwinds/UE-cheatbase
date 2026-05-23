#include "engine/EngineAdapter.hpp"
#include "core/Logger.hpp"

namespace cheatbase::engine {

EngineAdapter::EngineAdapter(const process::Process& proc, const process::MemoryReader& memory)
    : m_Process(proc), m_Memory(memory) {}

bool EngineAdapter::Initialize(std::string_view moduleName) {
    if (!ResolveOffsets(moduleName)) {
        CHEATBASE_ERROR("EngineAdapter: Failed to resolve core engine patterns.");
        return false;
    }
    return true;
}

UWorld EngineAdapter::GetWorld() const {
    if (!m_GWorld) return UWorld(0);
    
    // GWorld is typically a pointer to the UWorld pointer
    core::Address worldPtr = m_Memory.Read<core::Address>(m_GWorld);
    return UWorld(worldPtr);
}

bool EngineAdapter::ResolveOffsets(std::string_view moduleName) {
    process::PatternScanner scanner(m_Process, m_Memory);
    
    // Example signatures for UE5
    auto namePoolAddr = scanner.FindPattern(moduleName, "48 8D 05 ? ? ? ? EB 13 48 8D 0D ? ? ? ? E8");
    if (namePoolAddr) {
        m_NamePool = scanner.ResolveRelativeAddress(*namePoolAddr, 3, 7).value_or(0);
    }

    auto gWorldAddr = scanner.FindPattern(moduleName, "48 8B 05 ? ? ? ? 48 85 C0 74 ? 48 8B 48 08");
    if (gWorldAddr) {
        m_GWorld = scanner.ResolveRelativeAddress(*gWorldAddr, 3, 7).value_or(0);
    }

    auto gObjectsAddr = scanner.FindPattern(moduleName, "48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB");
    if (gObjectsAddr) {
        m_GObjects = scanner.ResolveRelativeAddress(*gObjectsAddr, 3, 7).value_or(0);
    }
    
    CHEATBASE_INFO("Engine Offsets Resolved:");
    CHEATBASE_INFO("  NamePool: 0x{:X}", m_NamePool);
    CHEATBASE_INFO("  GWorld:   0x{:X}", m_GWorld);
    CHEATBASE_INFO("  GObjects: 0x{:X}", m_GObjects);

    return m_NamePool && m_GWorld;
}

} // namespace cheatbase::engine

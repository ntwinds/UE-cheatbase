#pragma once

#include "process/Process.hpp"
#include "process/MemoryReader.hpp"
#include "process/PatternScanner.hpp"
#include "engine/EngineOffsets.hpp"
#include "engine/UWorld.hpp"

namespace cheatbase::engine {

class EngineAdapter {
public:
    EngineAdapter(const process::Process& proc, const process::MemoryReader& memory);

    bool Initialize(std::string_view moduleName);

    CHEATBASE_NODISCARD core::Address GetNamePool() const { return m_NamePool; }
    CHEATBASE_NODISCARD core::Address GetGObjects() const { return m_GObjects; }
    CHEATBASE_NODISCARD UWorld GetWorld() const;

private:
    const process::Process& m_Process;
    const process::MemoryReader& m_Memory;
    
    core::Address m_NamePool{0};
    core::Address m_GObjects{0};
    core::Address m_GWorld{0};

    bool ResolveOffsets(std::string_view moduleName);
};

} // namespace cheatbase::engine

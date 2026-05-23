#pragma once

#include "core/Types.hpp"
#include "process/MemoryReader.hpp"
#include <string>

namespace cheatbase::engine {

class FName {
public:
    explicit FName(core::u32 index, core::u32 number = 0) : ComparisonIndex(index), Number(number) {}

    CHEATBASE_NODISCARD std::string ToString(const process::MemoryReader& memory, core::Address namePoolAddress) const;

    core::u32 ComparisonIndex;
    core::u32 Number;
};

} // namespace cheatbase::engine

#pragma once

#include "core/Types.hpp"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace cheatbase::platform {

class WindowsPlatform {
public:
    static bool Initialize();
    static void Shutdown();

    static std::wstring Utf8ToWide(std::string_view utf8String);
    static std::string WideToUtf8(std::wstring_view wideString);
    
    // Abstracting OS errors
    static std::string GetLastErrorMessage();
};

} // namespace cheatbase::platform

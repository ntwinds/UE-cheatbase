#include "platform/WindowsPlatform.hpp"
#include "core/Logger.hpp"

namespace cheatbase::platform {

bool WindowsPlatform::Initialize() {
    // Windows specific initialization (e.g., DPI awareness)
    SetProcessDPIAware();
    return true;
}

void WindowsPlatform::Shutdown() {
}

std::wstring WindowsPlatform::Utf8ToWide(std::string_view utf8String) {
    if (utf8String.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &utf8String[0], (int)utf8String.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8String[0], (int)utf8String.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string WindowsPlatform::WideToUtf8(std::wstring_view wideString) {
    if (wideString.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideString[0], (int)wideString.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wideString[0], (int)wideString.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::string WindowsPlatform::GetLastErrorMessage() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) return std::string();

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    
    // Remove newlines from standard windows errors
    while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
        message.pop_back();
    }
    return message;
}

} // namespace cheatbase::platform

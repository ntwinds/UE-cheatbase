#include "core/Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace cheatbase::core {

std::mutex Logger::s_Mutex;

void Logger::Init() {
    // Platform specific console setup could go here (e.g., AllocConsole on Windows)
}

void Logger::Shutdown() {
    // Cleanup
}

void Logger::LogInternal(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(s_Mutex);

    // Get current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
#ifdef CHEATBASE_PLATFORM_WINDOWS
    localtime_s(&now_tm, &now_c);
#else
    localtime_r(&now_c, &now_tm);
#endif

    const char* levelStr = "";
    switch (level) {
        case LogLevel::Trace:   levelStr = "[TRACE]"; break;
        case LogLevel::Info:    levelStr = "[INFO] "; break;
        case LogLevel::Warning: levelStr = "[WARN] "; break;
        case LogLevel::Error:   levelStr = "[ERROR]"; break;
        case LogLevel::Fatal:   levelStr = "[FATAL]"; break;
    }

    std::cout << std::put_time(&now_tm, "[%H:%M:%S] ") << levelStr << " " << message << std::endl;
}

} // namespace cheatbase::core

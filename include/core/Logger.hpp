#pragma once

#include "core/Macros.hpp"
#include "core/Types.hpp"
#include <format>
#include <iostream>
#include <mutex>

namespace cheatbase::core {

enum class LogLevel : u8 {
    Trace,
    Info,
    Warning,
    Error,
    Fatal
};

class Logger {
public:
    static void Init();
    static void Shutdown();

    template<typename... Args>
    static void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
        std::string message = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(level, message);
    }

private:
    static void LogInternal(LogLevel level, const std::string& message);
    
    static std::mutex s_Mutex;
};

} // namespace cheatbase::core

// Convenience macros
#define CHEATBASE_TRACE(...)    ::cheatbase::core::Logger::Log(::cheatbase::core::LogLevel::Trace, __VA_ARGS__)
#define CHEATBASE_INFO(...)     ::cheatbase::core::Logger::Log(::cheatbase::core::LogLevel::Info, __VA_ARGS__)
#define CHEATBASE_WARN(...)     ::cheatbase::core::Logger::Log(::cheatbase::core::LogLevel::Warning, __VA_ARGS__)
#define CHEATBASE_ERROR(...)    ::cheatbase::core::Logger::Log(::cheatbase::core::LogLevel::Error, __VA_ARGS__)
#define CHEATBASE_FATAL(...)    ::cheatbase::core::Logger::Log(::cheatbase::core::LogLevel::Fatal, __VA_ARGS__)

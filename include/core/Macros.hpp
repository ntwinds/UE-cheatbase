#pragma once

#define CHEATBASE_INLINE __forceinline
#define CHEATBASE_NODISCARD [[nodiscard]]
#define CHEATBASE_DEPRECATED [[deprecated]]

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
#else
    #error "CheatBase currently only supports Windows."
#endif

// Assertion macro
#if defined(_DEBUG) || !defined(NDEBUG)
    #define CHEATBASE_ENABLE_ASSERTS
#endif

#ifdef CHEATBASE_ENABLE_ASSERTS
    #define CHEATBASE_ASSERT(x, msg) { if(!(x)) { __debugbreak(); } }
#else
    #define CHEATBASE_ASSERT(x, msg)
#endif

#define CHEATBASE_BIT(x) (1 << (x))

# UE-CheatBase
**Universal Engine Base (External)**

UE-CheatBase is a professional-grade modular external tooling platform designed for Unreal Engine 4 and Unreal Engine 5. It acts as an out-of-process engine abstraction framework, built specifically for AAA internal tooling, proprietary middleware, and enterprise engine infrastructure development.

> **Note:** CheatBase operates completely externally. It does not inject DLLs, nor does it hook internal engine functions.

## Architecture
CheatBase employs a strict layered architecture based on SOLID principles and modern C++20 features.

1. **Core / Platform Layer**: Foundational memory primitives, macros, singletons, and thread-safe abstractions.
2. **Process Layer**: Windows Handle abstraction, intelligent RTTI memory reading, and advanced SIMD-capable pattern scanning.
3. **Engine Abstraction Layer (EAL)**: High-level dynamic wrappers for UObject, UWorld, FName, and Object Arrays that automatically resolve offsets for UE4/UE5.
4. **Renderer / UI**: A stream-proof DX11 hardware-accelerated overlay utilizing the industry-standard Dear ImGui framework with a custom enterprise theme.
5. **Runtime**: Thread-safe Event Bus and dynamic module loader.

## Repository Structure
The repository features a clean, flattened hierarchy:

```
CheatBase/
 ├── include/         # All public and internal headers
 │   ├── core/        # Macros, standard types, logging
 │   ├── engine/      # Unreal Engine reflection adapters
 │   ├── platform/    # OS-specific wrappers (Windows API)
 │   ├── process/     # Process handle management and memory backends
 │   ├── renderer/    # DX11 Overlay & rendering primitives
 │   └── ui/          # Dear ImGui Panel interfaces
 ├── src/             # All implementation source files
 ├── CMakeLists.txt   # Unified build script
 └── README.md
```

## Building CheatBase
CheatBase uses CMake for cross-platform dependency management. Dear ImGui and nlohmann_json are fetched automatically during configuration.

1. Install **CMake** 3.20+
2. Generate the project:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
3. Open `CheatBase.sln` in Visual Studio and build in `Release` or `Debug`.

## Coding Guidelines
- **C++20 Minimum**: heavily utilize std::optional, std::expected, and smart pointers.
- **No Globals**: Use Dependency Injection or the specialized Singleton template.
- **Interfaces**: High-level systems must be abstracted behind abstract classes.

## License
MIT License. See LICENSE for more information.

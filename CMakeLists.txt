cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(CheatBase VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use solution folders for IDEs like Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Setup Output Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile options for professional enterprise grade SDK
if(MSVC)
    add_compile_options(/W4 /EHsc /permissive- /Zc:__cplusplus /utf-8 /MP)
    add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN CHEATBASE_PLATFORM_WINDOWS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter)
endif()

# Fetch external dependencies (e.g., nlohmann_json for config)
include(FetchContent)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

# Create static library for ImGui
add_library(imgui_lib STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
)
target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the main executable
add_executable(CheatBaseApp ${SOURCES})

# Include directories
target_include_directories(CheatBaseApp PRIVATE include)

# Link dependencies
target_link_libraries(CheatBaseApp PRIVATE nlohmann_json::nlohmann_json imgui_lib)
if(MSVC)
    target_link_libraries(CheatBaseApp PRIVATE d3d11 dxgi dwmapi)
endif()

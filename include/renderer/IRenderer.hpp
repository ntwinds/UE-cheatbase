#pragma once

#include "core/Types.hpp"
#include <string>
#include <vector>

namespace cheatbase::renderer {

struct Color {
    core::u8 R, G, B, A;
    
    static Color White() { return {255, 255, 255, 255}; }
    static Color Black() { return {0, 0, 0, 255}; }
    static Color Red() { return {255, 0, 0, 255}; }
    static Color Green() { return {0, 255, 0, 255}; }
    static Color Blue() { return {0, 0, 255, 255}; }
};

struct Vector2 {
    float X, Y;
};

// Abstract renderer interface for DX11, DX12, Vulkan
class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual bool Initialize(void* windowHandle) = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void DrawLine(Vector2 start, Vector2 end, Color color, float thickness = 1.0f) = 0;
    virtual void DrawRect(Vector2 pos, Vector2 size, Color color, float thickness = 1.0f) = 0;
    virtual void DrawRectFilled(Vector2 pos, Vector2 size, Color color) = 0;
    virtual void RenderText(Vector2 pos, std::string_view text, Color color, float fontSize = 16.0f) = 0;
};

} // namespace cheatbase::renderer

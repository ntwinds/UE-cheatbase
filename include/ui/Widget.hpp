#pragma once

#include "renderer/IRenderer.hpp"
#include <string>
#include <vector>
#include <memory>

namespace cheatbase::ui {

class Widget {
public:
    virtual ~Widget() = default;

    virtual void Draw(renderer::IRenderer& renderer) = 0;
    virtual void Update(float deltaTime) = 0;

    void SetPosition(renderer::Vector2 pos) { m_Position = pos; }
    void SetSize(renderer::Vector2 size) { m_Size = size; }
    
    renderer::Vector2 GetPosition() const { return m_Position; }
    renderer::Vector2 GetSize() const { return m_Size; }

protected:
    renderer::Vector2 m_Position{0, 0};
    renderer::Vector2 m_Size{100, 20};
};

} // namespace cheatbase::ui

#pragma once

#include "ui/Widget.hpp"
#include <vector>
#include <memory>
#include <string>

namespace cheatbase::ui {

class Panel : public Widget {
public:
    explicit Panel(std::string_view title);

    void AddChild(std::shared_ptr<Widget> widget);

    void Draw(renderer::IRenderer& renderer) override;
    void Update(float deltaTime) override;

private:
    std::string m_Title;
    std::vector<std::shared_ptr<Widget>> m_Children;
    
    // Styling
    renderer::Color m_BackgroundColor{20, 20, 22, 240}; // Dark enterprise theme
    renderer::Color m_TitleColor{200, 200, 200, 255};
};

} // namespace cheatbase::ui

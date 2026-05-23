#include "ui/Panel.hpp"
#include <imgui.h>

namespace cheatbase::ui {

Panel::Panel(std::string_view title)
    : m_Title(title) {
    m_Size = {600, 450}; // Default size for professional menu
}

void Panel::AddChild(std::shared_ptr<Widget> widget) {
    m_Children.push_back(std::move(widget));
}

void Panel::Draw(renderer::IRenderer& renderer) {
    (void)renderer; // Unused, ImGui handles rendering

    // Optional: Only apply style once globally in a real app, doing it here for simplicity
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]       = ImVec4(0.08f, 0.08f, 0.08f, 0.96f);
    colors[ImGuiCol_Border]         = ImVec4(0.24f, 0.24f, 0.26f, 0.65f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_TitleBg]        = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgActive]  = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_CheckMark]      = ImVec4(0.40f, 0.90f, 0.40f, 1.00f);
    colors[ImGuiCol_SliderGrab]     = ImVec4(0.40f, 0.90f, 0.40f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]=ImVec4(0.50f, 0.98f, 0.50f, 1.00f);
    colors[ImGuiCol_Button]         = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
    colors[ImGuiCol_Header]         = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
    colors[ImGuiCol_Separator]      = ImVec4(0.24f, 0.24f, 0.26f, 1.00f);
    colors[ImGuiCol_Tab]            = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TabHovered]     = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
    colors[ImGuiCol_TabActive]      = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_TabUnfocused]   = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);

    ImGui::SetNextWindowSize(ImVec2(m_Size.X, m_Size.Y), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(m_Title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
        
        if (ImGui::BeginTabBar("MenuTabs")) {
            if (ImGui::BeginTabItem("Aimbot")) {
                static bool enableAimbot = false;
                static float fov = 90.0f;
                static int smooth = 5;
                
                ImGui::Spacing();
                ImGui::Checkbox("Enable Aimbot", &enableAimbot);
                ImGui::SliderFloat("FOV Circle", &fov, 10.0f, 360.0f, "%.1f deg");
                ImGui::SliderInt("Smoothing", &smooth, 1, 20);
                ImGui::Separator();
                ImGui::TextDisabled("Aimbot features will appear here.");
                
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Visuals")) {
                static bool enableESP = true;
                static bool showBones = false;
                
                ImGui::Spacing();
                ImGui::Checkbox("Enable ESP", &enableESP);
                ImGui::Checkbox("Show Skeleton", &showBones);
                ImGui::Separator();
                ImGui::TextDisabled("Visual features will appear here.");
                
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Misc")) {
                ImGui::Spacing();
                ImGui::TextDisabled("Miscellaneous features.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings")) {
                ImGui::Spacing();
                ImGui::Text("CheatBase SDK v1.0.0");
                ImGui::TextDisabled("Enterprise Framework Architecture");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        
    }
    ImGui::End();
}

void Panel::Update(float deltaTime) {
    for (auto& child : m_Children) {
        child->Update(deltaTime);
    }
}

} // namespace cheatbase::ui

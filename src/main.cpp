#include "core/Logger.hpp"
#include "platform/WindowsPlatform.hpp"
#include "process/Process.hpp"
#include "process/MemoryReader.hpp"
#include "process/UsermodeBackend.hpp"
#include "process/DriverBackend.hpp"
#include "process/SharedMemoryBackend.hpp"
#include "engine/EngineAdapter.hpp"
#include "renderer/Overlay.hpp"
#include "renderer/DX11Renderer.hpp"
#include "ui/Panel.hpp"
#include <thread>
#include <chrono>
#include <imgui.h>

using namespace cheatbase;

int main() {
    core::Logger::Init();
    CHEATBASE_INFO("Starting CheatBase SDK Bootstrap...");

    platform::WindowsPlatform::Initialize();

    process::UsermodeBackend memoryBackend;

    process::Process proc;
    if (!proc.Attach("notepad.exe", &memoryBackend)) {
        CHEATBASE_ERROR("Waiting for Notepad...");
    }

    process::MemoryReader memory(proc);
    engine::EngineAdapter engine(proc, memory);

    if (proc.IsValid()) {
        if (!engine.Initialize("notepad.exe")) {
            CHEATBASE_WARN("Failed to resolve engine offsets.");
        }
    }

    CHEATBASE_INFO("Initializing Renderer and Overlay...");
    renderer::Overlay overlay;
    if (!overlay.Create("Untitled - Notepad")) {
        CHEATBASE_ERROR("Failed to create overlay.");
        return 1;
    }

    renderer::DX11Renderer renderer;
    if (!renderer.Initialize(overlay.GetWindowHandle())) {
        CHEATBASE_ERROR("Failed to initialize DX11.");
        return 1;
    }

    auto mainPanel = core::CreateRef<ui::Panel>("CheatBase Diagnostics");
    mainPanel->SetPosition({50, 50});

    CHEATBASE_INFO("Bootstrap complete. Entering main loop.");
    
    bool insertPressed = false;
    
    while (overlay.IsRunning()) {
        overlay.ProcessMessages(); // Let overlay process window messages
        
        bool keyPressed = (GetAsyncKeyState(VK_INSERT) & 0x8000) || (GetAsyncKeyState(VK_HOME) & 0x8000);
        if (keyPressed) {
            if (!insertPressed) {
                overlay.ToggleInteraction();
                insertPressed = true;
            }
        } else {
            insertPressed = false;
        }

        renderer.BeginFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        if (ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoFocusOnAppearing)) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "CheatBase Attached | Menu Hotkey: INSERT or HOME");
        }
        ImGui::End();

        if (overlay.IsInteractive()) {
            mainPanel->Update(0.016f);
            mainPanel->Draw(renderer);
        }

        renderer.EndFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    CHEATBASE_INFO("Shutting down CheatBase...");
    renderer.Shutdown();
    overlay.Destroy();
    proc.Detach();
    platform::WindowsPlatform::Shutdown();
    
    return 0;
}

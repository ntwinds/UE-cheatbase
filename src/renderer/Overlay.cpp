#include "renderer/Overlay.hpp"
#include "core/Logger.hpp"
#include "platform/WindowsPlatform.hpp"
#include <thread>
#include <chrono>
#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

namespace cheatbase::renderer {

Overlay::Overlay() = default;

Overlay::~Overlay() {
    Destroy();
}

bool Overlay::Create(std::string_view targetWindowName) {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    std::wstring wName = platform::WindowsPlatform::Utf8ToWide("CheatBaseOverlayClass");
    std::wstring targetName = platform::WindowsPlatform::Utf8ToWide(targetWindowName);

    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, wName.c_str(), nullptr };
    ::RegisterClassExW(&wc);

    HWND targetHwnd = FindWindowW(nullptr, targetName.c_str());
    if (!targetHwnd) {
        CHEATBASE_ERROR("Overlay: Target window '{}' not found.", targetWindowName);
        return false;
    }

    DWORD exStyle = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW;
    if (!m_Interactive) {
        exStyle |= WS_EX_TRANSPARENT;
    }

    m_Hwnd = ::CreateWindowExW(
        exStyle,
        wName.c_str(), wName.c_str(),
        WS_POPUP,
        0, 0, 1920, 1080,
        nullptr, nullptr, wc.hInstance, this
    );

    if (!m_Hwnd) {
        CHEATBASE_ERROR("Overlay: Failed to create window.");
        return false;
    }

    HWND hwnd = static_cast<HWND>(m_Hwnd);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
    
    // Make window completely transparent to DWM (click-through)
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Stream proofing (hides from OBS/Discord screenshare)
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

    UpdateOverlayPosition(targetHwnd);
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    m_Running = true;
    return true;
#else
    return false;
#endif
}

void Overlay::Destroy() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (m_Hwnd) {
        ::DestroyWindow(static_cast<HWND>(m_Hwnd));
        ::UnregisterClassW(L"CheatBaseOverlayClass", GetModuleHandle(nullptr));
        m_Hwnd = nullptr;
    }
#endif
    m_Running = false;
}

void Overlay::ProcessMessages() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT) {
            m_Running = false;
        }
    }
#endif
}

#ifdef CHEATBASE_PLATFORM_WINDOWS
LRESULT CALLBACK Overlay::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    if (msg == WM_DESTROY) {
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hwnd, msg, wParam, lParam);
}

void Overlay::ToggleInteraction() {
    m_Interactive = !m_Interactive;
    HWND hwnd = static_cast<HWND>(m_Hwnd);
    long style = GetWindowLong(hwnd, GWL_EXSTYLE);
    
    if (m_Interactive) {
        style &= ~WS_EX_TRANSPARENT; // Remove transparent so we can click
        SetForegroundWindow(hwnd);
    } else {
        style |= WS_EX_TRANSPARENT;  // Make transparent to click through
    }
    SetWindowLong(hwnd, GWL_EXSTYLE, style);
}

void Overlay::UpdateOverlayPosition(HWND targetHwnd) {
    RECT rect;
    GetWindowRect(targetHwnd, &rect);
    SetWindowPos(static_cast<HWND>(m_Hwnd), HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
}
#endif

} // namespace cheatbase::renderer

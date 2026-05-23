#pragma once

#include "core/Types.hpp"
#include <string>

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace cheatbase::renderer {

class Overlay {
public:
    Overlay();
    ~Overlay();

    bool Create(std::string_view targetWindowName);
    void Destroy();
    void ProcessMessages();

    bool IsRunning() const { return m_Running; }
    void Close() { m_Running = false; }

    bool IsInteractive() const { return m_Interactive; }
    void ToggleInteraction();

    void* GetWindowHandle() const { return m_Hwnd; }

private:
    bool m_Running{false};
    bool m_Interactive{true};
    void* m_Hwnd{nullptr};

#ifdef CHEATBASE_PLATFORM_WINDOWS
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void UpdateOverlayPosition(HWND targetHwnd);
#endif
};

} // namespace cheatbase::renderer

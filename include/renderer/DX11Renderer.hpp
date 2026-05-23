#pragma once

#include "renderer/IRenderer.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <d3d11.h>
#include <wrl/client.h>
#undef DrawText
#endif

namespace cheatbase::renderer {

class DX11Renderer : public IRenderer {
public:
    DX11Renderer();
    ~DX11Renderer() override;

    bool Initialize(void* windowHandle) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    void DrawLine(Vector2 start, Vector2 end, Color color, float thickness = 1.0f) override;
    void DrawRect(Vector2 pos, Vector2 size, Color color, float thickness = 1.0f) override;
    void DrawRectFilled(Vector2 pos, Vector2 size, Color color) override;
    void RenderText(Vector2 pos, std::string_view text, Color color, float fontSize = 16.0f) override;

private:
#ifdef CHEATBASE_PLATFORM_WINDOWS
    Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
#endif

    bool CreateDeviceAndSwapChain(void* windowHandle);
    bool CreateRenderTarget();
};

} // namespace cheatbase::renderer

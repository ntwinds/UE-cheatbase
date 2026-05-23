#include "renderer/DX11Renderer.hpp"
#include "core/Logger.hpp"

#ifdef CHEATBASE_PLATFORM_WINDOWS
#include <d3d11.h>
#include <wrl/client.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#undef DrawText
#endif

namespace cheatbase::renderer {

DX11Renderer::DX11Renderer() = default;

DX11Renderer::~DX11Renderer() {
    Shutdown();
}

bool DX11Renderer::Initialize(void* windowHandle) {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    if (!CreateDeviceAndSwapChain(windowHandle)) {
        CHEATBASE_ERROR("DX11Renderer: Failed to create device and swapchain.");
        return false;
    }
    if (!CreateRenderTarget()) {
        CHEATBASE_ERROR("DX11Renderer: Failed to create render target.");
        return false;
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(m_Device.Get(), m_DeviceContext.Get());
    
    CHEATBASE_INFO("DX11Renderer initialized successfully.");
    return true;
#else
    return false;
#endif
}

void DX11Renderer::Shutdown() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    
    m_RenderTargetView.Reset();
    m_SwapChain.Reset();
    m_DeviceContext.Reset();
    m_Device.Reset();
#endif
}

void DX11Renderer::BeginFrame() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif
}

void DX11Renderer::EndFrame() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    ImGui::Render();
    m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // Fully transparent
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_SwapChain->Present(1, 0); // VSync ON
#endif
}

void DX11Renderer::DrawLine(Vector2 start, Vector2 end, Color color, float thickness) {
    // Implementation requires custom shaders or Direct2D interop
    // Left as architecture stub for AAA framework
}

void DX11Renderer::DrawRect(Vector2 pos, Vector2 size, Color color, float thickness) {
    // Stub
}

void DX11Renderer::DrawRectFilled(Vector2 pos, Vector2 size, Color color) {
    // Stub
}

void DX11Renderer::RenderText(Vector2 pos, std::string_view text, Color color, float fontSize) {
    (void)pos; (void)text; (void)color; (void)fontSize;
    // Needs DirectWrite or Font Atlas system
}

bool DX11Renderer::CreateDeviceAndSwapChain(void* windowHandle) {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = static_cast<HWND>(windowHandle);
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &featureLevel, &m_DeviceContext);
    
    return SUCCEEDED(res);
#else
    return false;
#endif
}

bool DX11Renderer::CreateRenderTarget() {
#ifdef CHEATBASE_PLATFORM_WINDOWS
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
    if (FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) return false;
    
    HRESULT res = m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_RenderTargetView);
    return SUCCEEDED(res);
#else
    return false;
#endif
}

} // namespace cheatbase::renderer

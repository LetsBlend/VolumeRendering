#include "pch.h"
#include "D3D11.h"
#include "Log.h"
#include "Engine.h"
#include "Window/Window.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef DEBUG
#define SET_DEBUG_NAME(device, deviceName) SetDebugName(device, deviceName);
#else
#define SET_DEBUG_NAME(device, deviceName)
#endif

extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

template<typename T, UINT TDebugNameLength>
inline void SetDebugName(_In_ T deviceResource, _In_z_ const char(&debugName)[TDebugNameLength])
{
    deviceResource->SetPrivateData(WKPDID_D3DDebugObjectName, TDebugNameLength - 1, debugName);
}

bool D3D11::ShutDown()
{
    _deviceContext->Flush();

    _swapChain.Reset();
    _dxgiFactory.Reset();
    _deviceContext.Reset();

#ifdef DEBUG
    _debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
    _debug.Reset();
#endif
    _device.Reset();
    return true;
}

bool D3D11::StartUp()
{
    // Create DX Factory
    DEBUG_ASSERT(SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory))), "DXGI: Unable to create DXGI factory");
    HRESULT CreateDXGIFactory1(REFIID riid, void** ppFactory);

    // Create DX Device
    constexpr D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    uint32_t deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif

    DEBUG_ASSERT(SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, &deviceFeatureLevel, 1, D3D11_SDK_VERSION, &_device, nullptr, &_deviceContext)),
        "D3D11: Failed to create DX11 Device");

    SET_DEBUG_NAME(_device.Get(), "DEV_Main");
    SET_DEBUG_NAME(_deviceContext.Get(), "DEVCONTEXT_Main");

#ifdef DEBUG
    DEBUG_ASSERT(SUCCEEDED(_device.As(&_debug)), "D3D11: Failed to get the debug layer from the device");
#endif

    // Create SwapChain Descriptions
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = Window::GetWidth();
    swapChainDesc.Height = Window::GetHeight();
    swapChainDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
    swapChainDesc.Flags = Engine::VSync ? 0 : DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = {};
    swapChainFullscreenDesc.Windowed = true;
    swapChainFullscreenDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_STRETCHED;

    // Create SwapChain
    DEBUG_ASSERT(SUCCEEDED(_dxgiFactory->CreateSwapChainForHwnd(_device.Get(), Window::GetWindowHandle(), &swapChainDesc, &swapChainFullscreenDesc, nullptr, &_swapChain)),
        "DXGI: Failed to create SwapChain");


    Log::Trace("D3D11: Initialization completed!");
    return true;
}

void D3D11::OnResize(unsigned int width, unsigned int height)
{
    ID3D11RenderTargetView* nullRTV = nullptr;
    D3D11::_deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
    D3D11::_deviceContext->Flush();

    // Resize Buffers
    DEBUG_ASSERT(SUCCEEDED(D3D11::_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM, Engine::VSync ? 0 : DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING)), "D3D11: Failed to recreate SwapChain buffers");
}

void D3D11::Present()
{
    UINT presentFlag = Engine::VSync ? 0 : DXGI_PRESENT_ALLOW_TEARING;
    _swapChain->Present(Engine::VSync ? 1 : 0, presentFlag);

    _deviceContext->ClearState();
}

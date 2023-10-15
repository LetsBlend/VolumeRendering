#pragma once
#include "pch.h"

class D3D11
{
    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    D3D11() {}
    ~D3D11() {}

    bool StartUp();
    bool ShutDown();
    void OnResize(unsigned int width, unsigned int height);
    void Present();

    inline static ComPtr<IDXGIFactory2> _dxgiFactory = nullptr;
    inline static ComPtr<ID3D11Device> _device = nullptr;
    inline static ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
    inline static ComPtr<IDXGISwapChain1> _swapChain = nullptr;
    inline static ComPtr<ID3D11Debug> _debug = nullptr;
};
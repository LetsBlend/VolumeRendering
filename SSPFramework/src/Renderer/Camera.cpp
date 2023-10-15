#include "pch.h"
#include "Camera.h"
#include "Log.h"
#include "Window/Window.h"
#include "D3D11.h"
#include "Math/Transform.h"
#include <DirectXMath.h>

Camera::Camera()
{
    Load();
}

Camera::~Camera()
{
}

void Camera::Load()
{
    CreateConstantBuffer();
    CreateRenderTarget();
    CreateDepthTarget();
}

void Camera::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.ByteWidth = sizeof(CameraConstantBuffer);
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

    int result = D3D11::_device->CreateBuffer(&bufferDesc, nullptr, &m_cameraConstantBuffer);
    DEBUG_ASSERT(SUCCEEDED(result), "Failed to create Camera constant Buffer: ", result);
}

void Camera::CreateRenderTarget()
{
    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    int result = D3D11::_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to get BackBuffer from SwapChain: ", result);

    result = D3D11::_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to create Render Target View from back Buffer: ", result);
}

void Camera::CreateDepthTarget()
{
    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Height = Window::GetHeight();
    texDesc.Width = Window::GetWidth();
    texDesc.ArraySize = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.MipLevels = 1;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texDesc.Format = DXGI_FORMAT_R32_TYPELESS;

    ID3D11Texture2D* texture = nullptr;
    int result = D3D11::_device->CreateTexture2D(&texDesc, nullptr, &texture);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to create Depth Texture: ", result);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStenViewDesc{};
    depthStenViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStenViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    result = D3D11::_device->CreateDepthStencilView(texture, &depthStenViewDesc, &m_depthTargetView);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to create Depth Target: ", result);

    texture->Release();
}

void Camera::OnResize()
{
    CreateRenderTarget();
    CreateDepthTarget();

    UpdateProjectionMatrix();
    UpdateViewProjectionMatrix();
}

void Camera::DestroyResources()
{
    m_depthTargetView.Reset();
    m_renderTargetView.Reset();
}

void Camera::Update(Transform& trans)
{
    UpdateViewMatrx(trans);
    UpdateProjectionMatrix();
    UpdateViewProjectionMatrix();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    D3D11::_deviceContext->Map(m_cameraConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_cameraConstantBufferData, sizeof(CameraConstantBuffer));
    D3D11::_deviceContext->Unmap(m_cameraConstantBuffer.Get(), 0);
}

void Camera::UpdateViewMatrx(Transform& trans)
{
    Math::vec3 position = trans.GetPosition();
    Math::vec3 forward = trans.GetForward();
    Math::vec3 up = trans.GetUp();

    Math::vec3 lookAt = position + forward;

    Math::mat4x4 view = Math::lookAtLH(position, lookAt, up);

    m_cameraConstantBufferData.cameraPos = Math::vec4(position.x, position.y, position.z, 1);
    m_cameraConstantBufferData.viewMatrix = view;
    m_cameraConstantBufferData.inverseViewMatrix = Math::inverse(view);
}

void Camera::UpdateProjectionMatrix()
{
    Math::mat4x4 project = Math::perspectiveFovLH(Math::radians(60.0f), (float)Window::GetWidth(), (float)Window::GetHeight(), 0.1f, 1000.0f);
    
    m_cameraConstantBufferData.projectionMatrix = project;
    m_cameraConstantBufferData.inverseProjectionMatrix = Math::inverse(project);
}

void Camera::UpdateViewProjectionMatrix()
{
    Math::mat4x4 viewProject = m_cameraConstantBufferData.projectionMatrix * m_cameraConstantBufferData.viewMatrix;

    m_cameraConstantBufferData.viewProjectionMatrix = viewProject;
    m_cameraConstantBufferData.inverseViewProjectionMatrix = Math::inverse(viewProject);
}

void Camera::Clear()
{
	constexpr float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

    // Clear Depth Buffer
    D3D11::_deviceContext->ClearDepthStencilView(m_depthTargetView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Clear Normals Buffer
    D3D11::_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void Camera::Flush()
{
    m_cameraConstantBuffer.Reset();
    m_renderTargetView.Reset();
    m_depthTargetView.Reset();
}

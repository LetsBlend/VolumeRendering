#pragma once
#include "Math/Math.h"
#include <DirectXMath.h>

struct CameraConstantBuffer
{
    Math::mat4x4 viewMatrix;
    Math::mat4x4 projectionMatrix;
    Math::mat4x4 viewProjectionMatrix;

    Math::mat4x4 inverseViewMatrix;
    Math::mat4x4 inverseProjectionMatrix;
    Math::mat4x4 inverseViewProjectionMatrix;

    Math::vec4 cameraPos;
};
class Transform;

class Camera
{
    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    Camera();
    ~Camera();

    void Load();
    void CreateConstantBuffer();
    void CreateRenderTarget();
    void CreateDepthTarget();

    void OnResize();
    void DestroyResources();

    void Update(Transform& trans);
    void UpdateViewMatrx(Transform& trans);
    void UpdateProjectionMatrix();
    void UpdateViewProjectionMatrix();

    void Clear();
    void Flush();

    ComPtr<ID3D11Buffer> m_cameraConstantBuffer = nullptr;
    CameraConstantBuffer m_cameraConstantBufferData{};

    ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
    ComPtr<ID3D11DepthStencilView> m_depthTargetView = nullptr;
};


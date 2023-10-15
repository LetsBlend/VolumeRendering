#pragma once
#include "Math/Math.h"

class Material;
class Camera;
class Transform; 

struct ObjectConstantBuffer
{
    Math::mat4x4 modelMatrix;
    Math::mat4x4 inverseModelMatrix;
};

class MeshRenderer
{
    template<typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
    MeshRenderer(Material* mat, const char* filePath);
    ~MeshRenderer();

    void Load(const char* filePath);
    void LoadModel(const char* filePath);
    void CreateConstantBuffers();
    void Update(Transform& trans);
    void Render(Camera& cam);
    void Flush();

private:

    Material* material;

    ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
    ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;

    ComPtr<ID3D11Buffer> m_objectConstantBuffer = nullptr;
    ObjectConstantBuffer m_objectConstantBufferData{};

    uint32_t m_modelVertexCount = 0;
    uint32_t m_modelIndexCount = 0;
};
    
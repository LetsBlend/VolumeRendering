#include "pch.h"
#include "MeshRenderer.h"
#include "Log.h"
#include "Math/Math.h"
#include "Math/Transform.h"
#include "Window/Window.h"
#include "D3D11.h"
#include "ShaderFactory.h"
#include "Material.h"
#include "Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using Position = Math::vec3;
using Color = Math::vec3;
using Uv = Math::vec2;

struct VertexPositionColorUv
{
    Position position;
    Uv uv;
};

MeshRenderer::MeshRenderer(Material* mat, const char* filePath)
{
    material = mat;
    Load(filePath);
}

MeshRenderer::~MeshRenderer()
{
    Flush();
}

void MeshRenderer::Load(const char* filePath)
{
    LoadModel(filePath);
    CreateConstantBuffers();
}

void MeshRenderer::LoadModel(const char* filePath)
{
    constexpr uint32_t importFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

    Assimp::Importer sceneImporter;
    const aiScene* scene = sceneImporter.ReadFile(filePath, importFlags);

    DEBUG_ASSERT(scene != nullptr, "ASSIMP: Failed to load model file");
    
    DEBUG_ASSERT(scene->HasMeshes(), "ASSIMP: Model file is empty");

    const aiMesh* mesh = scene->mMeshes[0];
    DEBUG_ASSERT(mesh->HasPositions(), "ASSIMP: Model mesh has no position");

    constexpr Uv defaultUv = Uv{ 0, 0 };
    std::vector<VertexPositionColorUv> vertices;
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        const Position& position = Position{ mesh->mVertices[i].x / 2, mesh->mVertices[i].y / 2, mesh->mVertices[i].z / 2 };
        const Uv& uv = mesh->HasTextureCoords(0) ? Uv{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y } : defaultUv;

        vertices.push_back(VertexPositionColorUv{ Position{position}, Uv{uv} });
    }

    D3D11_BUFFER_DESC vertBufferDesc{};
    vertBufferDesc.ByteWidth = static_cast<uint32_t>(sizeof(VertexPositionColorUv) * vertices.size());
    vertBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA resourceData{};
    resourceData.pSysMem = vertices.data();

    int result = D3D11::_device->CreateBuffer(&vertBufferDesc, &resourceData, &m_vertexBuffer);
    DEBUG_ASSERT(SUCCEEDED(result), "Failed to create vertex Buffer: ", result);

    std::vector<uint32_t> indices;
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    D3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.ByteWidth = static_cast<uint32_t>(sizeof(uint32_t) * indices.size());
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    resourceData.pSysMem = indices.data();

    result = D3D11::_device->CreateBuffer(&indexBufferDesc, &resourceData, &m_indexBuffer);
    DEBUG_ASSERT(SUCCEEDED(result), "Failed to create index Buffer: ", result);

    m_modelIndexCount = (uint32_t)indices.size();
}

void MeshRenderer::CreateConstantBuffers()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.ByteWidth = sizeof(ObjectConstantBuffer);
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

    int result = D3D11::_device->CreateBuffer(&bufferDesc, nullptr, &m_objectConstantBuffer);
    DEBUG_ASSERT(SUCCEEDED(result), "Failed to create model constant Buffers: ", result);
}

void MeshRenderer::Update(Transform& trans)
{
    Math::mat4x4 model = Math::mat4x4(1.0f); // Initialize the model matrix as the identity matrix

    model = Math::translate(model, trans.GetPosition());
    model = Math::rotate(model, Math::radians(trans.GetPitchYawRoll().x), Math::vec3(1.0f, 0.0f, 0.0f));
    model = Math::rotate(model, Math::radians(trans.GetPitchYawRoll().y), Math::vec3(0.0f, 1.0f, 0.0f));
    model = Math::rotate(model, Math::radians(trans.GetPitchYawRoll().z), Math::vec3(0.0f, 0.0f, 1.0f));
    model = Math::scale(model, trans.GetScale()); 
    m_objectConstantBufferData.modelMatrix = model;
    m_objectConstantBufferData.inverseModelMatrix = Math::inverse(model);

    ////////////////////////////////////
    // Update Constant Buffers
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    D3D11::_deviceContext->Map(m_objectConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &m_objectConstantBufferData, sizeof(ObjectConstantBuffer));
    D3D11::_deviceContext->Unmap(m_objectConstantBuffer.Get(), 0);
    ////////////////////////////////////
}

void MeshRenderer::Render(Camera& cam)
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)Window::GetWidth();
    viewport.Height = (float)Window::GetHeight();
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;

    constexpr UINT vertexStride = sizeof(VertexPositionColorUv);
    constexpr UINT vertexOffset = 0;

    // The input assembler stage
    D3D11::_deviceContext->IASetInputLayout(material->m_inputLayout.Get());
    D3D11::_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
    D3D11::_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
    D3D11::_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // The vertex shader stage
    ID3D11Buffer* constantBuffers[2]{
        cam.m_cameraConstantBuffer.Get(),
        m_objectConstantBuffer.Get()
    };

    D3D11::_deviceContext->VSSetConstantBuffers(0, 2, constantBuffers);
    D3D11::_deviceContext->VSSetShader(material->m_vertexShader.Get(), nullptr, 0);

    // The rasterizer stage
    D3D11::_deviceContext->RSSetViewports(1, &viewport);
    D3D11::_deviceContext->RSSetState(material->m_rasterState.Get());

    // The pixel shader stage
    D3D11::_deviceContext->PSSetShader(material->m_pixelShader.Get(), nullptr, 0);

    // The output merger stage
    D3D11::_deviceContext->OMSetRenderTargets(1, cam.m_renderTargetView.GetAddressOf(), nullptr); // cam.m_depthTargetView.Get());
    //D3D11::_deviceContext->OMSetDepthStencilState(material->m_depthState.Get(), 0);

    // Present to the screen
    D3D11::_deviceContext->DrawIndexed(m_modelIndexCount, 0, 0);

    // Unbind every buffer
    ID3D11RenderTargetView* nullRTV = nullptr;
    D3D11::_deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
}

void MeshRenderer::Flush()
{
    m_vertexBuffer.Reset();
    m_indexBuffer.Reset();
    m_objectConstantBuffer.Reset();
}

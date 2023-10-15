#include "pch.h"
#include "Material.h"
#include "Log.h"
#include "Math/Math.h"
#include "D3D11.h"
#include "ShaderFactory.h"

Material::Material(const wchar_t* vsFilePath, const wchar_t* psFilePath)
{
    Load(vsFilePath, psFilePath);
}

Material::~Material()
{
    Flush();
}

void Material::Load(const wchar_t* vsFilePath, const wchar_t* psFilePath)
{
    ComPtr<ID3DBlob> vertexShaderBlob = nullptr;

    m_vertexShader = ShaderFactory::CreateVertexShader(vsFilePath, vertexShaderBlob);

    m_pixelShader = ShaderFactory::CreatePixelShader(psFilePath);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    ShaderFactory::CreateInputLayoutDesc(m_inputLayout, vertexShaderBlob, inputLayoutDesc);

    CreateRasterState();
    CreateDepthState();
}

void Material::CreateRasterState()
{
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.AntialiasedLineEnable = true;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0;
    rasterDesc.DepthClipEnable = false;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0;

    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;

    int result = D3D11::_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to create Rasterizer state: ", result);
}

void Material::CreateDepthState()
{
    D3D11_DEPTH_STENCIL_DESC depthDesc{};
    depthDesc.DepthEnable = true;
    depthDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    int result = D3D11::_device->CreateDepthStencilState(&depthDesc, &m_depthState);
    DEBUG_ASSERT(SUCCEEDED(result), "Failed to create Depth Stencil State: ", result);
}

void Material::Flush()
{
    m_rasterState.Reset();
    m_depthState.Reset();
    m_inputLayout.Reset();
    m_vertexShader.Reset();
    m_pixelShader.Reset();
}

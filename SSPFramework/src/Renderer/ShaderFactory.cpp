#include "pch.h"
#include "ShaderFactory.h"
#include "Log.h"
#include "D3D11.h"

ShaderFactory::ShaderFactory()
{
}

ShaderFactory::~ShaderFactory()
{
}

bool ShaderFactory::CompileShader(const wchar_t* fileName, const std::string& entryPoint, const std::string& profile, ComPtr<ID3DBlob>& shaderBlob)
{
#ifdef DEBUG
    constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    constexpr UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNES;
#endif

    // temporary shader blobs where compiled data gets stored
    ComPtr<ID3DBlob> tempShaderBlob = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.data(), profile.data(), compileFlags, 0, &tempShaderBlob, &errorBlob)))
    {
        if (errorBlob == nullptr)
        {
            DEBUG_ASSERT(0, "D3D11: Failed to read shader from file");
        }
        else
        {
            DEBUG_ASSERT(0, "D3D11: Failed to read shader from file: ", static_cast<const char*>(errorBlob->GetBufferPointer()));
        }
    }

    shaderBlob = std::move(tempShaderBlob);
    return true;
}

Microsoft::WRL::ComPtr<ID3D11ComputeShader> ShaderFactory::CreateComputeShader(const wchar_t* fileName, ComPtr<ID3DBlob>& computeShaderBlob)
{
    if (!CompileShader(fileName, "main", "cs_5_0", computeShaderBlob))
        return nullptr;

    ComPtr<ID3D11ComputeShader> computeShader;
    DEBUG_ASSERT(SUCCEEDED(D3D11::_device->CreateComputeShader(computeShaderBlob->GetBufferPointer(), computeShaderBlob->GetBufferSize(), nullptr, &computeShader)),
        "D3D11: Failed to compile vertex shader");

    return computeShader;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> ShaderFactory::CreateVertexShader(const wchar_t* fileName, ComPtr<ID3DBlob>& vertexShaderBlob)
{
    if (!CompileShader(fileName, "vert", "vs_5_0", vertexShaderBlob))
        return nullptr;

    ComPtr<ID3D11VertexShader> vertexShader;
    DEBUG_ASSERT(SUCCEEDED(D3D11::_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader)),
        "D3D11: Failed to compile vertex shader");

    return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> ShaderFactory::CreatePixelShader(const wchar_t* fileName)
{
    ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
    if (!CompileShader(fileName, "frag", "ps_5_0", pixelShaderBlob))
        return nullptr;

    ComPtr<ID3D11PixelShader> pixelShader;
    DEBUG_ASSERT(SUCCEEDED(D3D11::_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader)),
        "D3D11: Failed to compile pixel shader");

    return pixelShader;
}

DXGI_FORMAT MaskToFormat(D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);

void ShaderFactory::CreateInputLayoutDesc(ComPtr<ID3D11InputLayout>& inputLayout, ComPtr<ID3DBlob>& shaderBlob, std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDesc)
{
    ID3D11ShaderReflection* shaderReflection = nullptr;

    int result = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&shaderReflection);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to Create shader Reflection: ", result);

    D3D11_SHADER_DESC shaderDesc;
    shaderReflection->GetDesc(&shaderDesc);

    for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        shaderReflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;
        elementDesc.Format = MaskToFormat(paramDesc);

        inputLayoutDesc.push_back(elementDesc);
    }

    result = D3D11::_device->CreateInputLayout(inputLayoutDesc.data(), (UINT)inputLayoutDesc.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
    DEBUG_ASSERT(SUCCEEDED(result), "D3D11: Failed to create default vertex input layout: ", result);
}

DXGI_FORMAT MaskToFormat(D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
{
    if (paramDesc.Mask == 1)
    {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32_FLOAT;
    }
    else if (paramDesc.Mask <= 3)
    {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32_FLOAT;
    }
    else if (paramDesc.Mask <= 7)
    {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32_FLOAT;
    }
    else if (paramDesc.Mask <= 15)
    {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32A32_UINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32A32_SINT;
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    DEBUG_ASSERT(0, "Sora: Failed to create Input Layout! No matching DXGI_FORMAT found for specified Mask!");
    return DXGI_FORMAT_R32_UINT;
}
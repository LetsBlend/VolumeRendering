#pragma once
#include "pch.h"

class ShaderFactory
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	ShaderFactory();
	~ShaderFactory();

	static bool CompileShader(const wchar_t* fileName, const std::string& entryPoint, const std::string& profile, ComPtr<ID3DBlob>& shaderBlob);
	static ComPtr<ID3D11ComputeShader> CreateComputeShader(const wchar_t* fileName, ComPtr<ID3DBlob>& vertexShaderBlob);
	static ComPtr<ID3D11VertexShader> CreateVertexShader(const wchar_t* fileName, ComPtr<ID3DBlob>& vertexShaderBlob);
	static ComPtr<ID3D11PixelShader> CreatePixelShader(const wchar_t* fileName);
	static void CreateInputLayoutDesc(ComPtr<ID3D11InputLayout>& inputLayout, ComPtr<ID3DBlob>& shaderBlob, std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutDesc);
};


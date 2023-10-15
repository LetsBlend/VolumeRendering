#pragma once
class Material
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	Material(const wchar_t* vsFilePath, const wchar_t* psFilePath);
	~Material();

	void Load(const wchar_t* vsFilePath, const wchar_t* psFilePath);
	void CreateRasterState();
	void CreateDepthState();
	void Flush();

	ComPtr<ID3D11RasterizerState> m_rasterState = nullptr;
	ComPtr<ID3D11DepthStencilState> m_depthState = nullptr;

	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
};


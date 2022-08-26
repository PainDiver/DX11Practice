#pragma once
#include "Object.h"

enum
{
	RASTERIZER_DEPTH_DEFAULT,
	RASTERIZER_DEPTH_SKYMAP,
	RASTERIZER_DEPTH_WIREFRAME,
	RASTERIZER_DEPTH_UI,
	RASTERIZER_DEPTH_MAX
};

class Shader : public Object
{
public:
	Shader(const WCHAR* vs, const WCHAR* ps, uint8 rasterizerDepthState);

	HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	ComPtr <ID3D11RasterizerState> GetRasterizerState() { return _currentRasterizerState; }
	ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return _currentDepthStencilState; }
	ComPtr<ID3D11BlendState> GetBlendState() { return _currentBlendState; }


	ComPtr < ID3D11InputLayout> GetInputLayout() { return _inputLayout; }
	ComPtr<ID3D11VertexShader> GetVertexShader() { return _vertexShader; }
	ComPtr<ID3D11PixelShader> GetPixelShader() { return _pixelShader; }
	ComPtr < ID3D11SamplerState> GetSamplerState() { return _samplerState; }

private:

	ComPtr < ID3D11SamplerState> _samplerState;

	ComPtr < ID3D11VertexShader> _vertexShader;
	ComPtr < ID3D11PixelShader> _pixelShader;
	ComPtr < ID3D11InputLayout> _inputLayout;

	ComPtr < ID3D11DepthStencilState> _currentDepthStencilState;
	ComPtr < ID3D11RasterizerState> _currentRasterizerState;
	ComPtr<ID3D11BlendState> _currentBlendState;

};


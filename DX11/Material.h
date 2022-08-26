#pragma once
#include "Object.h"
class Material :public Object
{
public:
	Material() {}
	Material(const WCHAR* path);

	ComPtr<ID3D11ShaderResourceView> GetTexture() { return _texture; }

	void SetMaterial(ComPtr <ID3D11ShaderResourceView> srv) { _texture = srv; }

private:
	
	ComPtr<ID3D11ShaderResourceView> _texture;
};


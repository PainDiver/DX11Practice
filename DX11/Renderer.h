#pragma once

enum class ERenderType
{
	DEFAULT,
	SKYMAP,
	INSTANCE,
	UI,
	MAX
};

class Mesh;
class Material;
class Shader;
#include "Component.h"

class Renderer :public Component
{
public:

	Renderer(ERenderType rt = ERenderType::DEFAULT, UINT instancingNumber = 1) :Component(EComponent::Renderer) { _renderType = rt; _instancingNumber = instancingNumber; }
	
	void ShadowRender();
	
	void Render()override;


	void RenderDefault();
	void RenderInstance();
	void RenderSkyMap();
	void RenderUI();
	void RenderShadowMap();
	void RenderEnd();


	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material.push_back(material); }
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }


private:

	ERenderType _renderType;

	ComPtr < ID3D11SamplerState> _samplerState;

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr < ID3D11PixelShader> _pixelShader;
	ComPtr < ID3D11InputLayout> _inputLayout;

	shared_ptr<Shader> _shader;
	shared_ptr<Mesh> _mesh;
	vector<shared_ptr<Material>> _material;

	UINT _instancingNumber;
};


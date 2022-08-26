#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"



void ResourceManager::MakeResources()
{
    shared_ptr<Mesh> rectangle = make_shared<Mesh>(
        RectSpace::vertices,
        RectSpace::indices,
        sizeof(Vertex),
        RectSpace::vertexSize,
        RectSpace::indexSize);

    shared_ptr<Mesh> plane = make_shared<Mesh>(
        PlaneSpace::vertices,
        PlaneSpace::indices,
        sizeof(Vertex),
        PlaneSpace::vertexSize,
        PlaneSpace::indexSize);


    shared_ptr<Shader> skyMapShader = make_shared<Shader>(L"../HLSL/SkyMap.fx", L"../HLSL/SkyMap.fx", RASTERIZER_DEPTH_SKYMAP);
    shared_ptr<Shader> defaultShader = make_shared<Shader>(L"../HLSL/DefaultShader.fx", L"../HLSL/DefaultShader.fx", RASTERIZER_DEPTH_DEFAULT);
    shared_ptr<Shader> wireFrameShader = make_shared<Shader>(L"../HLSL/DefaultShader.fx", L"../HLSL/DefaultShader.fx", RASTERIZER_DEPTH_WIREFRAME);
    shared_ptr<Shader> uiShader = make_shared<Shader>(L"../HLSL/UIShader.fx", L"../HLSL/UIShader.fx", RASTERIZER_DEPTH_UI);
    shared_ptr<Shader> shadowMapShader = make_shared<Shader>(L"../HLSL/ShadowMapShader.fx", L"../HLSL/ShadowMapShader.fx", RASTERIZER_DEPTH_DEFAULT);


    shared_ptr<Material> material = make_shared<Material>(L"Tutorial07.jpg");
    shared_ptr<Material> normal = make_shared<Material>(L"Tutorial07Normal.png");
    shared_ptr<Material> skymapMaterial = make_shared<Material>(L"SkyMap.jpg");
    shared_ptr<Material> ground = make_shared<Material>(L"Ground.jpg");
    shared_ptr<Material> groundNormal = make_shared<Material>(L"GroundNormal.jpg");


    GET_SINGLE(ResourceManager)->AddResource("Ground", ground);
    GET_SINGLE(ResourceManager)->AddResource("GroundNormal", groundNormal);


    GET_SINGLE(ResourceManager)->AddResource("UIShader", uiShader);
    GET_SINGLE(ResourceManager)->AddResource("WireFrameShader", wireFrameShader);
    GET_SINGLE(ResourceManager)->AddResource("SkyMapShader", skyMapShader);
    GET_SINGLE(ResourceManager)->AddResource("DefaultShader", defaultShader);
    GET_SINGLE(ResourceManager)->AddResource("ShadowMapShader", shadowMapShader);

    
    GET_SINGLE(ResourceManager)->AddResource("BoxMaterial", material);
    GET_SINGLE(ResourceManager)->AddResource("BoxNormal", normal);
    GET_SINGLE(ResourceManager)->AddResource("SkyMapTexture", skymapMaterial);
    GET_SINGLE(ResourceManager)->AddResource("Rectangle", rectangle);
    GET_SINGLE(ResourceManager)->AddResource("Plane", plane);


}


void ResourceManager::RemoveResource(string name)
{
	if (_resources.find(name) == _resources.end())
	{
		return;
	}
	_resources.erase(name);
}

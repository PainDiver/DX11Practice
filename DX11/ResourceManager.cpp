#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "FBXLoader.h"


void ResourceManager::MakeResources()
{
    vector<Vertex> v;
    vector<WORD> i;
    GET_SINGLE(FBXLoader)->AddScene("emmy.fbx");
    GET_SINGLE(FBXLoader)->GetInputSet(0, v, i);
    Vertex* vArray = &v[0];
    WORD* iArray = &i[0];

    shared_ptr<Mesh> emmy = make_shared<Mesh>(
        vArray,
        iArray,
        sizeof(Vertex),
        v.size(),
        i.size());

    v.clear();
    i.clear();

    //GET_SINGLE(FBXLoader)->AddScene("dancer.fbx");
    //GET_SINGLE(FBXLoader)->GetInputSet(1, v, i);
    //vArray = &v[0];
    //iArray = &i[0];

    //shared_ptr<Mesh> dancer = make_shared<Mesh>(
    //    vArray,
    //    iArray,
    //    sizeof(Vertex),
    //    v.size(),
    //    i.size());

    //v.clear();
    //i.clear();

    //GET_SINGLE(FBXLoader)->AddScene("Dragon.fbx");
    //GET_SINGLE(FBXLoader)->GetInputSet(2, v, i);
    //vArray = &v[0];
    //iArray = &i[0];

    //shared_ptr<Mesh> dragon = make_shared<Mesh>(
    //    vArray,
    //    iArray,
    //    sizeof(Vertex),
    //    v.size(),
    //    i.size());



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
    shared_ptr<Material> dancerTex = make_shared<Material>(L"dancerTex.jpg");
    shared_ptr<Material> dancerNormal = make_shared<Material>(L"dancerNormal.jpg");
    shared_ptr<Material> emmyTex = make_shared<Material>(L"emmyTex.png");
    shared_ptr<Material> emmyNormal = make_shared<Material>(L"emmyNormal.png");
    shared_ptr<Material> dragonTex = make_shared<Material>(L"DragonTex.jpg");
    shared_ptr<Material> dragonNormal = make_shared<Material>(L"DragonNormal.jpg");


    

    GET_SINGLE(ResourceManager)->AddResource("UIShader", uiShader);
    GET_SINGLE(ResourceManager)->AddResource("WireFrameShader", wireFrameShader);
    GET_SINGLE(ResourceManager)->AddResource("SkyMapShader", skyMapShader);
    GET_SINGLE(ResourceManager)->AddResource("DefaultShader", defaultShader);
    GET_SINGLE(ResourceManager)->AddResource("ShadowMapShader", shadowMapShader);

    
    GET_SINGLE(ResourceManager)->AddResource("BoxMaterial", material);
    GET_SINGLE(ResourceManager)->AddResource("BoxNormal", normal);
    GET_SINGLE(ResourceManager)->AddResource("SkyMapTexture", skymapMaterial);
    GET_SINGLE(ResourceManager)->AddResource("Ground", ground);
    GET_SINGLE(ResourceManager)->AddResource("GroundNormal", groundNormal);
    GET_SINGLE(ResourceManager)->AddResource("DancerTex", dancerTex);
    GET_SINGLE(ResourceManager)->AddResource("DancerNormal", dancerNormal);
    GET_SINGLE(ResourceManager)->AddResource("EmmyTex", emmyTex);
    GET_SINGLE(ResourceManager)->AddResource("EmmyNormal", emmyNormal);
    GET_SINGLE(ResourceManager)->AddResource("DragonTex", dragonTex);
    GET_SINGLE(ResourceManager)->AddResource("DragonNormal", dragonNormal);


    GET_SINGLE(ResourceManager)->AddResource("Rectangle", rectangle);
    GET_SINGLE(ResourceManager)->AddResource("Plane", plane);
    GET_SINGLE(ResourceManager)->AddResource("Emmy", emmy);
    //GET_SINGLE(ResourceManager)->AddResource("Dancer", dancer);
    //GET_SINGLE(ResourceManager)->AddResource("Dragon", dragon);


}


void ResourceManager::RemoveResource(string name)
{
	if (_resources.find(name) == _resources.end())
	{
		return;
	}
	_resources.erase(name);
}

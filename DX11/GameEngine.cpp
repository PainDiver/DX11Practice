#include "pch.h"
#include "GameEngine.h"
#include "Camera.h"
#include "Renderer.h"
#include "GameScene.h"
#include "Transform.h"
#include "TestScript.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "Shader.h"

void GameEngine::Init()
{
	_device->Init(_window);

    GET_SINGLE(Timer)->Init();
    GET_SINGLE(Input)->Init();

    GET_SINGLE(ResourceManager)->MakeResources();

    _currentScene = make_shared<GameScene>();


#pragma region SkyMap
    {
        shared_ptr<GameObject> skyMap = make_shared<GameObject>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<Renderer> renderer = make_shared<Renderer>(ERenderType::SKYMAP);
        
        shared_ptr<Material> material = GET_SINGLE(ResourceManager)->GetResource<Material>("SkyMapTexture");
        shared_ptr<Mesh> mesh = GET_SINGLE(ResourceManager)->GetResource<Mesh>("Rectangle");
        shared_ptr<Shader> shader = GET_SINGLE(ResourceManager)->GetResource<Shader>("SkyMapShader");

        renderer->SetMaterial(material);
        renderer->SetMesh(mesh);
        renderer->SetShader(shader);

        skyMap->AddComponent(transform);
        skyMap->AddComponent(renderer);
        _currentScene->AddGameObject(skyMap);
    }
#pragma endregion


#pragma region light
    {
        shared_ptr<GameObject> light = make_shared<GameObject>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<Light> lightComponent = make_shared<Light>();

        lightComponent->Init();
        light->AddComponent(transform);
        light->AddComponent(lightComponent);
        _currentScene->AddGameObject(light);
    }
#pragma endregion

#pragma region camera
    {
        shared_ptr<GameObject> cameraObject = make_shared<GameObject>();
        shared_ptr<Camera> cameraComponent = make_shared<Camera>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<TestScript> script = make_shared<TestScript>();
        cameraComponent->Init();
        transform->Init(0, 0, 0);
        cameraObject->AddComponent(cameraComponent);
        cameraObject->AddComponent(transform);
        cameraObject->AddComponent(script);
        _currentScene->AddGameObject(cameraObject);
    }
#pragma endregion

#pragma region Plane

    {
        shared_ptr<GameObject> Object = make_shared<GameObject>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<Renderer> renderer = make_shared<Renderer>(ERenderType::DEFAULT);

        shared_ptr<Shader> shader = GET_SINGLE(ResourceManager)->GetResource<Shader>("DefaultShader");
        shared_ptr<Mesh> mesh = GET_SINGLE(ResourceManager)->GetResource<Mesh>("Rectangle");
        shared_ptr<Material> material = GET_SINGLE(ResourceManager)->GetResource<Material>("Ground");
        shared_ptr<Material> normal = GET_SINGLE(ResourceManager)->GetResource<Material>("GroundNormal");

        transform->Init(0, -2, 20);
        transform->SetScale({ 80,1, 80 });
        renderer->SetMaterial(material);
        renderer->SetMaterial(normal);
        renderer->SetMesh(mesh);
        renderer->SetShader(shader);

        Object->AddComponent(transform);
        Object->AddComponent(renderer);

        _currentScene->AddGameObject(Object);
    }
#pragma endregion



#pragma region Object
    {
        shared_ptr<GameObject> Object = make_shared<GameObject>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<Renderer> renderer = make_shared<Renderer>(ERenderType::INSTANCE, 200);

        shared_ptr<Shader> shader = GET_SINGLE(ResourceManager)->GetResource<Shader>("DefaultShader");
        shared_ptr<Mesh> mesh = GET_SINGLE(ResourceManager)->GetResource<Mesh>("Rectangle");
        shared_ptr<Material> material = GET_SINGLE(ResourceManager)->GetResource<Material>("BoxMaterial");
        shared_ptr<Material> normal = GET_SINGLE(ResourceManager)->GetResource<Material>("BoxNormal");

        transform->Init(-48, 0, -48);
        renderer->SetMaterial(material);
        renderer->SetMaterial(normal);
        renderer->SetMesh(mesh);
        renderer->SetShader(shader);

        Object->AddComponent(transform);
        Object->AddComponent(renderer);

        _currentScene->AddGameObject(Object);
    }

#pragma region UI
    {
        shared_ptr<GameObject> Object = make_shared<GameObject>();
        shared_ptr<Transform> transform = make_shared<Transform>();
        shared_ptr<Renderer> renderer = make_shared<Renderer>(ERenderType::UI);

        shared_ptr<Shader> shader = GET_SINGLE(ResourceManager)->GetResource<Shader>("UIShader");
        shared_ptr<Mesh> mesh = GET_SINGLE(ResourceManager)->GetResource<Mesh>("Plane");
        shared_ptr<Material> material = GET_SINGLE(ResourceManager)->GetResource<Material>("BoxMaterial");
        
        shared_ptr<Material> rt = make_shared<Material>();
        rt->SetMaterial(DEVICECLASS->GetMultipleRenderTargetResource());

        transform->Init(-0.8, 0.6, 0);
        transform->SetScale({ 75,75, 75 });
        renderer->SetMaterial(rt);
        renderer->SetMesh(mesh);
        renderer->SetShader(shader);

        Object->AddComponent(transform);
        Object->AddComponent(renderer);

        _currentScene->AddGameObject(Object);
    }
#pragma endregion
}

void GameEngine::Update()
{
    GET_SINGLE(Input)->Update();

    if (GET_SINGLE(Timer)->Update())
    {
        wstring str = L"FPS:";
        int fps = GET_SINGLE(Timer)->GetFPS();
        POINT xy = GET_SINGLE(Input)->GetMousePos();
        MouseMoveState moveState = GET_SINGLE(Input)->GetMouseMoveState();
        str += std::to_wstring(fps) + L"Mouse: " + std::to_wstring(xy.x) + L"," + std::to_wstring(xy.y) + L"Mouse_Move: " + std::to_wstring(static_cast<int>(moveState._xEvent)) + L"," + std::to_wstring(static_cast<int>(moveState._yEvent));
        SetWindowText(GWINDOW._hwnd,str.c_str());
    }

    if (_currentScene)
    {
        _currentScene->Update();
        _currentScene->LastUpdate();
    }

    Render();
}

void GameEngine::RenderBegin()
{
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_window._width;
    vp.Height = (FLOAT)_window._height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;


    _device->GetContext()->RSSetViewports(1, &vp);
    Clear();
}

void GameEngine::Render()
{
    RenderBegin();

    if (_currentScene)
        _currentScene->Render();

    RenderEnd();
}

void GameEngine::RenderEnd()
{
    _device->GetSwapChain()->Present(0, 0);
}

void GameEngine::Clear()
{
    ComPtr <ID3D11RenderTargetView> rendertargetView = _device->GetRenderTargetView();
    ComPtr <ID3D11DepthStencilView> depthStencilView = _device->GetDepthStencilView();

    float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; // red, green, blue, alpha
    _device->GetContext()->ClearRenderTargetView(rendertargetView.Get(), ClearColor);
    _device->GetContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    ComPtr <ID3D11RenderTargetView> multipleRendertargetView = _device->GetMultipleRenderTargetView();
    ComPtr <ID3D11DepthStencilView> multipleDepthStencilView = _device->GetMultipleRenderDepthStencilView();
    _device->GetContext()->ClearDepthStencilView(multipleDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    _device->GetContext()->ClearRenderTargetView(multipleRendertargetView.Get(), ClearColor);
}

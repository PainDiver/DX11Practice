#include "pch.h"
#include "GameScene.h"
#include "GameEngine.h"

void GameScene::Update()
{
	for (auto gameObject : _gameObjects)
	{
		gameObject->Update();
	}

}

void GameScene::LastUpdate()
{
	for (auto gameObject : _gameObjects)
	{
		gameObject->LastUpdate();
	}

}

void GameScene::Render()
{
	for (auto gameObject : _gameObjects)
	{
		ComPtr <ID3D11RenderTargetView> rendertargetView = DEVICECLASS->GetMultipleRenderTargetView();
		ComPtr <ID3D11DepthStencilView> depthStencilView = DEVICECLASS->GetMultipleRenderDepthStencilView();
		CONTEXT->OMSetRenderTargets(1, rendertargetView.GetAddressOf(), depthStencilView.Get());
		gameObject->PreRender();
	}

	for (auto gameObject : _gameObjects)
	{
		ComPtr <ID3D11RenderTargetView> rendertargetView = DEVICECLASS->GetRenderTargetView();
		ComPtr <ID3D11DepthStencilView> depthStencilView = DEVICECLASS->GetDepthStencilView();
		CONTEXT->OMSetRenderTargets(1, rendertargetView.GetAddressOf(), depthStencilView.Get());

		gameObject->Render();
	}
}


void GameScene::AddGameObject(shared_ptr<GameObject> go)
{
	_gameObjects.push_back(go);
}
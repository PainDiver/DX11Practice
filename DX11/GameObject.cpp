#include "pch.h"
#include "GameObject.h"
#include "Renderer.h"

void GameObject::AddComponent(shared_ptr<Component> com)
{
	EComponent type = com->GetType();
	if (_components.find(type) == _components.end())
	{
		_components[type] = com;
		_components[type]->SetGameObject(shared_from_this());
	}
}

void GameObject::RemoveComponent(EComponent type)
{
	if (_components.find(type) == _components.end())
		return;

	_components.erase(type);
}

void GameObject::Update()
{
	for (auto component : _components)
	{
		component.second->Update();
	}

}

void GameObject::LastUpdate()
{
	for (auto component : _components)
	{
		component.second->LastUpdate();
	}


}

void GameObject::PreRender()
{
	if (_components.find(EComponent::Renderer) == _components.end())
	{
		return;
	}
	static_pointer_cast<Renderer>(_components[EComponent::Renderer])->ShadowRender();
}

void GameObject::Render()
{
	if (_components.find(EComponent::Renderer) == _components.end())
	{
		return;
	}
	_components[EComponent::Renderer]->Render();
}

shared_ptr<Component> GameObject::GetComponent(EComponent type)
{

	if (_components.find(type) == _components.end())
		return nullptr;

	return _components[type];
}

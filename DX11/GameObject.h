#pragma once
#include "Component.h"

class GameObject : public enable_shared_from_this<GameObject>
{
public:

	void AddComponent(shared_ptr<Component> com);
	
	void RemoveComponent(EComponent type);


	
	void Update();

	void LastUpdate();

	void PreRender();

	void Render();


	shared_ptr<Component> GetComponent(EComponent type);

private:
	unordered_map<EComponent, shared_ptr<Component>> _components;
};


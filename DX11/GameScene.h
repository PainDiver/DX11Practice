#pragma once
#include "GameObject.h"

class GameScene
{
public:
	void Update();

	void LastUpdate();

	void Render();

	void AddGameObject(shared_ptr<GameObject> go);

private:

	vector<shared_ptr<GameObject>> _gameObjects;
};


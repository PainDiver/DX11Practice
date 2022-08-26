#pragma once

class Transform;
class GameObject;

class Component
{
public:

	Component(EComponent type) :_type(type) {}

	virtual void Update() {}

	virtual void LastUpdate() {}

	virtual void Render() {}

	EComponent GetType() { return _type; }

	void SetGameObject(shared_ptr<GameObject> go) { _gameObject = go; }

	shared_ptr<GameObject> GetGameObject() { return _gameObject.lock(); }

	shared_ptr<Transform> GetTransform();

private:
	EComponent _type;

	weak_ptr<GameObject> _gameObject;
};


#pragma once
#include "Component.h"

class Transform: public Component
{
public:
	Transform();

	void Init(float x, float y, float z);

	void Update()override;

	XMMATRIX GetWorldMat() { return _worldMat; }

	void SetPos(const XMFLOAT3& t);
	void SetScale(const XMFLOAT3& s);
	void SetRotation(const XMFLOAT3& r);

	void SetOrigin(const XMFLOAT3& rot);

	XMFLOAT3 GetPos() { return pos; }
	XMFLOAT3 GetScale() { return _scale; }
	XMFLOAT3 GetRotation() { return _rot; }

	XMVECTOR GetForwardVector() { return _forwardVector; }
	XMVECTOR GetRightVector() { return _rightVector; }
	XMVECTOR GetUpVector() { return _upVector; }

	float GetWidth() { return _width; }


private:
	XMFLOAT3 pos;
	XMFLOAT3 _rot;
	XMFLOAT3 _scale;

	XMVECTOR _forwardVector;
	XMVECTOR _rightVector;
	XMVECTOR _upVector;

	XMMATRIX _worldMat;

	float _width;
};


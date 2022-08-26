#pragma once
#include "Component.h"

class LightCamera
{
public:
	XMMATRIX GetViewMat() { return _view; }
	XMMATRIX GetProjMat() {return _projection;}

	void SetViewMat(XMMATRIX viewMat) { _view = viewMat; }
	void SetProjMat(XMMATRIX projMat) { _projection = projMat; }


private:

	XMMATRIX _view;
	XMMATRIX _projection;
};


class Light :public Component
{
public:
	Light() :Component(EComponent::Light) {}

	void Init();
	void LastUpdate()override;

	static XMFLOAT4 GetDirection() { return _dir; }
	static XMFLOAT4 GetColor() { return _color; }

	XMMATRIX GetLightViewMat() { return _cam.GetViewMat(); }
	XMMATRIX GetLightProjMat() { return _cam.GetProjMat(); }

private:
	static XMFLOAT4 _dir;
	static XMFLOAT4 _color;

	LightCamera _cam;

	float _test;
	bool _turn;
};


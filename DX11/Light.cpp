#include "pch.h"
#include "Light.h"
#include "Timer.h"
#include "Transform.h"
#include "GameEngine.h"

XMFLOAT4 Light::_dir;
XMFLOAT4 Light::_color;

void Light::Init()
{
	_dir = { 1,-1,0,0 };
	
	XMVECTOR dir;
	dir = XMLoadFloat4(&_dir);
	dir = XMVector4Normalize(dir);
	XMStoreFloat4(&_dir, dir);

	_color ={ 1, 1, 1, 1 };

}

void Light::LastUpdate()
{
	// light cam update
	XMVECTOR dit;
	_cam.SetViewMat(XMMatrixInverse(&dit, GetTransform()->GetWorldMat()));
	_cam.SetProjMat(XMMatrixPerspectiveFovLH(XM_PI / 4, GWINDOW._width / (FLOAT)GWINDOW._height, 1.f, 1000.f));

}

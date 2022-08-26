#pragma once
#include "Component.h"

enum
{
	FRONT,
	BACK,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRUSTUM_MAX
};

struct Frustum
{
	array<XMVECTOR,6> _planes;
};

enum class CameraType
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};

class Camera : public Component
{
	

public:
	Camera() :Component(EComponent::Camera) {}

	void Init();

	void LastUpdate()override;

	void ConstructFrustum();

	static XMMATRIX GetWorldMat() { return _world; }
	static XMMATRIX GetViewMat() { return _view; }
	static XMMATRIX GetProjMat(CameraType type) { 
		if (type == CameraType::PERSPECTIVE)
			return _projection;
		else
			return _orthographicProjection;
	}


	static bool CheckFrustum(const XMFLOAT3& pos,float width);



private:

	static XMMATRIX _world;
	static XMMATRIX _view;
	static XMMATRIX _projection;
	static XMMATRIX _orthographicProjection;
	
	static Frustum _frustum;
};


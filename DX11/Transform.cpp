#include "pch.h"
#include "Transform.h"
#include "Camera.h"

Transform::Transform() : Component(EComponent::Transform)
{
	_pos = XMFLOAT3{ 0,0,0 };

	_rot = XMFLOAT3{ 0,0,0 };

	_scale = XMFLOAT3{ 1,1,1 };

	_width = max(_scale.x, _scale.z);

	_worldMat = XMMatrixIdentity();

	_forwardVector = XMVECTOR{ 0,0,1,0 };
	_rightVector = XMVECTOR{ 1,0,0,0 };
	_upVector = XMVECTOR{ 0,1,0,0 };
}

void Transform::Init(float x, float y, float z)
{
	_pos = XMFLOAT3{ x,y,z };

	_rot = XMFLOAT3{ 0,0,0};

	_scale = XMFLOAT3{ 1,1,1 };

	_forwardVector = XMVECTOR{ 0,0,1,0};
	_rightVector = XMVECTOR{ 1,0,0,0 };
	_upVector = XMVECTOR{ 0,1,0,0 };

	_width = max(_scale.x, _scale.z);
}

void Transform::Update()
{
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX translation = XMMatrixTranslation(_pos.x, _pos.y, _pos.z);
	XMMATRIX rotation = XMMatrixRotationX(_rot.x);
	rotation *=  XMMatrixRotationY(_rot.y);
	rotation *= XMMatrixRotationZ(_rot.z);


	_worldMat = scale * rotation * translation;

}

void Transform::SetPos(const XMFLOAT3& t)
{
	_pos = t;
}

void Transform::SetScale(const XMFLOAT3& s)
{
	_scale = s;
	_width = max(_scale.x, _scale.z);
}

void Transform::SetRotation(const XMFLOAT3& r)
{
	_rot = r;
	SetOrigin(r);
}

void Transform::SetOrigin(const XMFLOAT3& rot)
{
	XMMATRIX rotation = XMMatrixRotationX(_rot.x);
	rotation *= XMMatrixRotationY(_rot.y);
	rotation *= XMMatrixRotationZ(_rot.z);

	XMVECTOR forward = { 0,0,1,0 };
	_forwardVector = XMVector3Normalize(XMVector3TransformNormal(forward, rotation));

	XMVECTOR right = { 1,0,0,0 };
	_rightVector = XMVector3Normalize(XMVector3TransformNormal(right, rotation));

	XMVECTOR up = { 0,1,0,0 };
	_upVector = XMVector3Normalize(XMVector3TransformNormal(up, rotation));
}


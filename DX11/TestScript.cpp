#include "pch.h"
#include "TestScript.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"

void TestScript::Update()
{
	shared_ptr<Transform> transform = GetTransform();
	float delta = GET_SINGLE(Timer)->GetDeltaTime();
	
	if (GET_SINGLE(Input)->KeyState(EKeyType::W) == EKeyState::PRESEED)
	{
		XMFLOAT3 pos = transform->GetPos();
		XMVECTOR posVec = XMLoadFloat3(&pos);
		XMVECTOR vec = transform->GetForwardVector()* delta* _speed;

		posVec += vec;
		XMStoreFloat3(&pos, posVec);
		transform->SetPos( pos);
	}
	if (GET_SINGLE(Input)->KeyState(EKeyType::A) == EKeyState::PRESEED)
	{
		XMFLOAT3 pos = transform->GetPos();
		XMVECTOR posVec = XMLoadFloat3(&pos);
		XMVECTOR vec = -transform->GetRightVector() * delta * _speed;

		posVec += vec;
		XMStoreFloat3(&pos, posVec);
		transform->SetPos(pos);
	}
	if (GET_SINGLE(Input)->KeyState(EKeyType::S) == EKeyState::PRESEED)
	{
		XMFLOAT3 pos = transform->GetPos();
		XMVECTOR posVec = XMLoadFloat3(&pos);
		XMVECTOR vec = -transform->GetForwardVector() * delta * _speed;

		posVec += vec;
		XMStoreFloat3(&pos, posVec);
		transform->SetPos(pos);

	}
	if (GET_SINGLE(Input)->KeyState(EKeyType::D) == EKeyState::PRESEED)
	{

		XMFLOAT3 pos = transform->GetPos();
		XMVECTOR posVec = XMLoadFloat3(&pos);
		XMVECTOR vec = transform->GetRightVector() * delta * _speed;

		posVec += vec;
		XMStoreFloat3(&pos, posVec);
		transform->SetPos(pos);

	}
	MouseMoveState ms = GET_SINGLE(Input)->GetMouseMoveState();


	if (GET_SINGLE(Input)->KeyState(EKeyType::LEFT_BUTTON) == EKeyState::PRESEED)
	{
		if (ms._xEvent == EMouseEvent::LEFT)
		{
			XMFLOAT3 rot = transform->GetRotation();
			rot.y -= delta * ms._xDelta;
			transform->SetRotation(rot);
		}
		if (ms._xEvent == EMouseEvent::RIGHT)
		{
			XMFLOAT3 rot = transform->GetRotation();
			rot.y -= delta * ms._xDelta;
			transform->SetRotation(rot);
		}
		if (ms._yEvent == EMouseEvent::UP)
		{
			XMFLOAT3 rot = transform->GetRotation();
			rot.x -= delta * ms._yDelta;
			transform->SetRotation(rot);

		}
		if (ms._yEvent == EMouseEvent::DOWN)
		{
			XMFLOAT3 rot = transform->GetRotation();
			rot.x -= delta * ms._yDelta;
			transform->SetRotation(rot);
		}
	}

}

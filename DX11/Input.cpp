#include "pch.h"
#include "Input.h"
#include "GameEngine.h"

void Input::Init()
{
	for (auto keyType : EKeyType::all)
	{
		_key[keyType] = EKeyState::NONE;
	}
	_mouseMoveState = { EMouseEvent::NONE,EMouseEvent::NONE };
	
	POINT xy;
	GetCursorPos(&xy);
	_mousePos = {xy.x,xy.y};
	
	
}

void Input::Update()
{
	KeyUpdate();
	MouseUpdate();
}

void Input::KeyUpdate()
{
	for (auto keyType : EKeyType::all)
	{
		if (GetKeyState(keyType) < 0)
		{
			if (KeyState(keyType) == EKeyState::DOWN)
				_key[keyType] = EKeyState::PRESEED;
			else if (KeyState(keyType) == EKeyState::NONE)
				_key[keyType] = EKeyState::DOWN;
		}
		else
		{
			if (KeyState(keyType) == EKeyState::PRESEED || KeyState(keyType) == EKeyState::DOWN)
				_key[keyType] = EKeyState::UP;
			else if (KeyState(keyType) == EKeyState::UP)
				_key[keyType] = EKeyState::NONE;
		}
	}
}

void Input::MouseUpdate()
{
	POINT cursor;
	GetCursorPos(&cursor);
	InitializeState();


	if (_mousePos.x - cursor.x > 0 )
	{
		_mouseMoveState._xEvent = EMouseEvent::LEFT;
		_mouseMoveState._xDelta = _mousePos.x - cursor.x;
	}
	else if (_mousePos.x - cursor.x < 0)
	{
		_mouseMoveState._xEvent = EMouseEvent::RIGHT;
		_mouseMoveState._xDelta = _mousePos.x - cursor.x;
	}

	if (_mousePos.y - cursor.y < 0)
	{
		_mouseMoveState._yEvent = EMouseEvent::DOWN;
		_mouseMoveState._yDelta = _mousePos.y - cursor.y;
	}
	else if(_mousePos.y - cursor.y > 0)
	{
		_mouseMoveState._yEvent = EMouseEvent::UP;
		_mouseMoveState._yDelta = _mousePos.y - cursor.y;
	}

	_mousePos = { cursor.x,cursor.y };
}

void Input::InitializeState()
{
	_mouseMoveState._xEvent = EMouseEvent::NONE;
	_mouseMoveState._yEvent = EMouseEvent::NONE;
	_mouseMoveState._xDelta = 0;
	_mouseMoveState._yDelta = 0;

}

EKeyState Input::KeyState(EKeyType::EKey key)
{
	uint8 number = static_cast<uint8>(key);
	return _key[number];	
}



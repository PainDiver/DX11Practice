#pragma once

namespace EKeyType
{
	enum EKey :uint8
	{
		UP = VK_UP,
		DOWN = VK_DOWN,
		LEFT = VK_LEFT,
		RIGHT = VK_RIGHT,

		W = 'W',
		A = 'A',
		S = 'S',
		D = 'D',

		LEFT_BUTTON = VK_LBUTTON,
		RIGHT_BUTTON = VK_RBUTTON,
	};
	static const EKey all[] = { UP,DOWN,LEFT,RIGHT,W,A,S,D,LEFT_BUTTON,RIGHT_BUTTON};
}

enum class EKeyState
{
	NONE,
	DOWN,
	PRESEED,
	UP,
};


enum class EMouseEvent
{
	NONE,
	UP,
	LEFT,
	DOWN,
	RIGHT,
};

struct MouseMoveState
{
	EMouseEvent _xEvent;
	EMouseEvent _yEvent;
	int _xDelta;
	int _yDelta;
};

class Input
{
	DECLARE_SINGLE(Input)


public:

	void Init();

	void Update();

	void KeyUpdate();

	void MouseUpdate();

	EKeyState KeyState(EKeyType::EKey key);

	POINT GetMousePos() { return _mousePos; }

	MouseMoveState GetMouseMoveState() { return _mouseMoveState; }

	void InitializeState();

private:
	EKeyState _key[256];


	POINT _mousePos;
	MouseMoveState _mouseMoveState;
};


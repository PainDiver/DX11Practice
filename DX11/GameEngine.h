#pragma once

#include "Device.h"
#include "Buffers.h"
#include "Input.h"
#include "Timer.h"

class GameScene;
class GameEngine
{
	DECLARE_SINGLE(GameEngine)

public:

	void Init();

	void Update();

	void RenderBegin();

	void Render();

	void RenderEnd();
	
	void Clear();


	shared_ptr<Device> GetDevice() { return _device; }

	shared_ptr<Buffers> GetBuffer() { return _buffers; }

	void SetWindow(GWindow window) { _window=window; }

	GWindow GetWindow() { return _window; }


private:
	GWindow			   _window;
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<Buffers> _buffers = make_shared<Buffers>();

	shared_ptr<GameScene> _currentScene;
};


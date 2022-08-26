#pragma once
class Timer
{
	DECLARE_SINGLE(Timer)

public:
	void Init();

	bool Update();

	float GetDeltaTime() { return _deltaTime; }

	uint16 GetFPS() { return _fps; }

private:

	float _totalTime;

	uint16 tickCount;
	uint16 _fps;
	LARGE_INTEGER _freq;
	LARGE_INTEGER _prevCount;

	float _deltaTime;
};


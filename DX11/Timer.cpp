#include "pch.h"
#include "Timer.h"


void Timer::Init()
{
	QueryPerformanceFrequency(&_freq);
	QueryPerformanceCounter(&_prevCount);

	_totalTime;
}

bool Timer::Update()
{
	LARGE_INTEGER currentCounter;
	QueryPerformanceCounter(&currentCounter);

	_deltaTime = static_cast<float>((currentCounter.QuadPart - _prevCount.QuadPart) / static_cast<float>(_freq.QuadPart));
	_totalTime += _deltaTime;
	tickCount++;
	
	_prevCount = currentCounter;

	if (_totalTime >= 1.f)
	{
		_fps = tickCount;
		_totalTime = 0;
		tickCount = 0;

		return true;
	}

}



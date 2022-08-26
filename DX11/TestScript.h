#pragma once

#include "Script.h"

class TestScript : public Script
{
public:
	TestScript() :Script() {}

	void Update() override;

private:
	float _speed = 10.f;
	float _rotSpeed = 45.f;

};


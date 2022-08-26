#include "pch.h"
#include "GameEngine.h"

void Debug(HRESULT hr, HWND hwnd)
{
	std::string message = std::system_category().message(hr);
	MessageBoxA(hwnd, message.c_str(), nullptr, 0);
}

XMMATRIX MakeSRT(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale)
{
	XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX posMat = XMMatrixTranslation(pos.x, pos.y, pos.z);
	
	XMMATRIX rotMat = XMMatrixRotationX(rot.x);
	rotMat *= XMMatrixRotationY(rot.y);
	rotMat *= XMMatrixRotationZ(rot.z);


	return scaleMat * rotMat * posMat;
}

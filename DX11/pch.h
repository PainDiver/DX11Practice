#pragma once

#include <vector>
#include <unordered_map>
#include <list>
#include <array>
#include <memory>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <system_error>
#include <tchar.h>
#include <string>
#include <wrl.h>
#include <fbxsdk.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

#pragma comment(lib,"DirectXTex.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")



using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;


using int8 = __int8;
using uint8 = unsigned __int8;
using int16 = __int16;
using uint16 = unsigned __int16;
using int32 = __int32;
using uint32 = unsigned __int32;
using int64 = __int64;
using uint64 = unsigned __int64;


struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
};

struct MatInfo
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX WVP;
};

struct LightInfo
{
	XMFLOAT4 dir;
	XMFLOAT4 color;
};

struct Flags
{
	int instancing;
	int padding[3];
};

struct InstancingInfo
{
	XMMATRIX instanceWorld[200];
};



enum class EComponent :uint8
{
	Transform,
	Renderer,
	Camera,
	Light,
	Physics,
	Script,
};



struct GWindow
{
	HINSTANCE _hInst;
	HWND _hwnd;
	
	UINT _width;
	UINT _height;

};


#define DECLARE_SINGLE(type)			\
private:								\
	type() {}							\
public:									\
	static type* GetInstance()			\
	{									\
		static type instance;			\
		return &instance;				\
	}									\


#define GET_SINGLE(type) type::GetInstance()



#define GENGINE GameEngine::GetInstance()

#define GWINDOW GameEngine::GetInstance()->GetWindow()

#define CONTEXT GameEngine::GetInstance()->GetDevice()->GetContext()

#define DEVICE GameEngine::GetInstance()->GetDevice()->GetDevice()

#define DEVICECLASS GameEngine::GetInstance()->GetDevice()

#define CONSTANTBUFFER GameEngine::GetInstance()->GetBuffer()


void Debug(HRESULT hr, HWND hwnd);

XMMATRIX MakeSRT(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale);
#include "pch.h"
#include "Camera.h"
#include "GameEngine.h"
#include "Transform.h"

XMMATRIX Camera::_world;
XMMATRIX Camera::_view;
XMMATRIX Camera::_projection;
XMMATRIX Camera::_orthographicProjection;
Frustum Camera::_frustum;

void Camera::Init()
{
	_world = XMMatrixIdentity();
	_view = XMMatrixIdentity();
	_projection = XMMatrixIdentity();

}


void Camera::LastUpdate()
{
	_world = GetTransform()->GetWorldMat();
	
	XMVECTOR dit;
	_view = XMMatrixInverse(&dit, _world);
	
	_projection = XMMatrixPerspectiveFovLH(XM_PI / 4, GWINDOW._width/ (FLOAT)GWINDOW._height, 1.f, 1000.f);

	_orthographicProjection = XMMatrixOrthographicLH(GWINDOW._width, GWINDOW._height, 1.f, 1000.f);

	XMFLOAT3 pos = GetTransform()->GetPos();

	ConstructFrustum();
}

void Camera::ConstructFrustum()
{
	XMVECTOR dit;
	XMMATRIX viewInverse = XMMatrixInverse(&dit, (_view));
	XMMATRIX projInverse = XMMatrixInverse(&dit, (_projection));

	XMMATRIX viewProjInverse = projInverse * viewInverse;

	//뒷면
	XMFLOAT3 LDB{ -1,-1,1 }; // 왼아래
	XMFLOAT3 LUB{ -1,1,1 }; //왼위
	XMFLOAT3 RUB{ 1,1,1 }; // 오른위
	XMFLOAT3 RDB{ 1,-1,1 }; // 오른아래	
	
	//앞면
	XMFLOAT3 LDF{ -1,-1,0 }; //왼아래
	XMFLOAT3 LUF{ -1,1,0 };	//왼위
	XMFLOAT3 RUF{ 1,1,0 };	//오른위
	XMFLOAT3 RDF{ 1,-1,0 };	//오른아래

	XMVECTOR VLDF = XMLoadFloat3(&LDF);
	XMVECTOR VLUF = XMLoadFloat3(&LUF);
	XMVECTOR VRUF = XMLoadFloat3(&RUF);
	XMVECTOR VRDF = XMLoadFloat3(&RDF);

	XMVECTOR VLDB = XMLoadFloat3(&LDB);
	XMVECTOR VLUB = XMLoadFloat3(&LUB);
	XMVECTOR VRUB = XMLoadFloat3(&RUB);
	XMVECTOR VRDB = XMLoadFloat3(&RDB);

	VLDF = XMVector3TransformCoord(VLDF, viewProjInverse);
	VLUF = XMVector3TransformCoord(VLUF, viewProjInverse);
	VRUF = XMVector3TransformCoord(VRUF, viewProjInverse);
	VRDF = XMVector3TransformCoord(VRDF, viewProjInverse);
	VLDB = XMVector3TransformCoord(VLDB, viewProjInverse);
	VLUB = XMVector3TransformCoord(VLUB, viewProjInverse);
	VRUB = XMVector3TransformCoord(VRUB, viewProjInverse);
	VRDB = XMVector3TransformCoord(VRDB, viewProjInverse);

	_frustum._planes[FRONT] = XMPlaneFromPoints(VLDF,VLUF,VRUF);
	_frustum._planes[BACK] = XMPlaneFromPoints(VRUB,VLUB,VLDB);
	_frustum._planes[UP] = XMPlaneFromPoints(VLUF,VLUB,VRUB);
	_frustum._planes[DOWN] = XMPlaneFromPoints(VRDB,VLDB,VLDF);
	_frustum._planes[LEFT] = XMPlaneFromPoints(VLDB,VLUB,VLUF);
	_frustum._planes[RIGHT] = XMPlaneFromPoints(VRUF,VRUB,VRDB);

}


bool Camera::CheckFrustum(const XMFLOAT3& pos,float width)
{
	XMVECTOR position = XMLoadFloat3(&pos);

	for (int i = FRONT; i < FRUSTUM_MAX; i++)
	{
		float distance = XMVectorGetX(XMVector3Dot(position, _frustum._planes[i]));
		float d = XMVectorGetW(_frustum._planes[i]);

		if (distance + d > width)
			return false;
	}

	return true;
}

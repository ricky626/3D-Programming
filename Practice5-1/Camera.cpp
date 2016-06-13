#include "stdafx.h"
//#include "Player.h"
#include "Camera.h"

CCamera::CCamera()
{
	m_pPlayer = NULL;

	D3DXMatrixIdentity(&m_d3dxmtxView);
	D3DXMatrixIdentity(&m_d3dxmtxProjection);

	m_pd3dcbCamera = NULL;
}

CCamera::~CCamera()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();
}


//Direct3D 디바이스의 뷰-포트를 설정하기 위한 함수이다.
void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
	pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

// CCamera 클래스의 GenerateViewMatrix() 멤버 함수를 다음과 같이 정의한다.이 함수는 카메라 변환 행렬을 생성한다.카메라의 위치 벡터, 카메라가 바라보는 지점, 카메라의 Up 벡터(로컬 y - 축 벡터)를 파라메터로 사용하는 D3DXMatrixLookAtLH() 함수를 사용한다.
void CCamera::GenerateViewMatrix(D3DXVECTOR3 d3dxvEyePosition, D3DXVECTOR3 d3dxvLookAt, D3DXVECTOR3 d3dxvUp)
{
	D3DXMatrixLookAtLH(&m_d3dxmtxView, &d3dxvEyePosition, &d3dxvLookAt, &d3dxvUp);
}

// CCamera 클래스의 GenerateProjectionMatrix() 멤버 함수
//투영 변환 행렬을 생성하는 함수이다.카메라 좌표계의 근평면 거리(카메라에서 볼 수 있는 가장 가까운 거리), 원평면 거리(카메라에서 볼 수 있는 가장 먼 거리), 화면의 종횡비(Aspect Ratio), 카메라 FOV 각도를  위치 벡터, 카메라가 바라보는 지점, 카메라의 Up 벡터(로컬 y - 축 벡터)를 파라메터로 사용하는 D3DXMatrixPerspectiveFovLH() 함수를 사용한다.
void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_d3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

// CCamera 클래스의 CreateShaderVariables() 멤버 함수
// 카메라 변환 행렬과 투영 변환 행렬을 쉐이더로 전달하기 위한 구조체에 대한 상수 버퍼를 생성하는 함수이다.
void CCamera::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_CAMERA);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

// CCamera 클래스의 UpdateShaderVariables() 멤버 함수
//카메라 변환 행렬과 투영 변환 행렬을 쉐이더로 전달하기 위한 구조체에 대한 상수 버퍼를 갱신하고 디바이스 컨텍스트의 슬롯에 연결하는 함수이다.
void CCamera::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	/*상수 버퍼의 메모리 주소를 가져와서 카메라 변환 행렬과 투영 변환 행렬을 복사한다. 쉐이더에서 행렬의 행과 열이 바뀌는 것에 주의하라.*/
	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_CAMERA *pcbViewProjection = (VS_CB_CAMERA *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &m_d3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, &m_d3dxmtxProjection);
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	//상수 버퍼를 슬롯(VS_SLOT_CAMERA)에 설정한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_CAMERA, 1, &m_pd3dcbCamera);
}
#pragma once

#include "Camera.h"
#include "Mesh.h"

#define DIR_FORWARD	0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08
#define DIR_UP		0x10
#define DIR_DOWN	0x20

struct MATERIAL
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular; //(r,g,b,a=power)
	D3DXCOLOR m_d3dxcEmissive;
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

	int m_nReferences;
	void AddRef();
	void Release();

	MATERIAL m_Material;
};

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	CGameObject *m_pChild;
	//객체의 첫 번째 자식 객체에 대한 포인터이다. 
	CGameObject *m_pSibling;
	//객체의 형제 객체에 대한 포인터이다. 
	CGameObject *m_pParent;
	//객체의 부모 객체에 대한 포인터이다. 

	virtual void SetChild(CGameObject *pChild);

	//객체의 위치를 설정한다.
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	//로컬 x-축, y-축, z-축 방향으로 이동한다.
	virtual void MoveStrafe(float fDistance = 1.0f);
	virtual void MoveUp(float fDistance = 1.0f);
	virtual void MoveForward(float fDistance = 1.0f);

	//로컬 x-축, y-축, z-축 방향으로 회전한다.
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle);

	//객체의 위치, 로컬 x-축, y-축, z-축 방향 벡터를 반환한다.
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetLookAt();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
private:
	int m_nReferences;
public:
	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	void SetMaterial(CMaterial *pMaterial);
	virtual void SetMesh(CMesh *pMesh);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	//자전 속도와 회전축 벡터를 설정하는 함수이다.
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

private:
	//자전 속도와 회전축 벡터를 나타내는 멤버 변수를 선언한다.
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;
};

class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject();
	virtual ~CRevolvingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);

	// 공전 속력을 설정한다.
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	// 공전을 위한 회전축을 설정한다.
	void SetRevolutionAxis(D3DXVECTOR3 d3dxvRevolutionAxis) { m_d3dxvRevolutionAxis = d3dxvRevolutionAxis; }

private:
	// 공전 회전축과 회전 속력을 나타낸다.
	D3DXVECTOR3 m_d3dxvRevolutionAxis;
	float m_fRevolutionSpeed;
};

/////////////

class CHeightMap
{
private:
	//높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 각 픽셀은 0~255의 값을 갖는다.
	BYTE *m_pHeightMapImage;
	//높이 맵 이미지의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;
	//높이 맵을 실제로 몇 배 확대하여 사용할 것인가를 나타내는 스케일 벡터이다.
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale);
	~CHeightMap(void);

	//높이 맵 이미지에서 (x, z) 위치의 픽셀 값에 기반한 지형의 높이를 반환한다.
	float GetHeight(float x, float z, bool bReverseQuad = false);
	//높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다.
	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor);
	~CHeightMapTerrain();

	//지형의 높이 맵으로 사용할 이미지이다.
	CHeightMap *m_pHeightMap;

	//지형의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;

	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.
	D3DXVECTOR3 m_d3dxvScale;
	//지형 전체가 어떤 위치와 방향을 갖는 가를 나타내는 월드 변환 행렬이다.
	D3DXMATRIX m_d3dxmtxWorld;

	//지형을 구성하는 격자 메쉬들의 배열과 메쉬들의 개수이다.
	CHeightMapGridMesh **m_ppMesh;
	int m_nMeshes;

	void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera = NULL);
};

///////////////

class CGunObject : public CGameObject
{
public:
	CGunObject();
	virtual ~CGunObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetRotationAngle(float fRotationAngle) { m_fRotationAngle = fRotationAngle; }
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }

private:
	//포신 객체의 현재 회전 각도를 나타낸다. 포신은 위-아래 방향(y-축)으로 정해진 각도 내에서만 회전한다.
	float	m_fRotationAngle;
	//포신 객체의 회전 속력(1초 동안 회전하는 각도)를 나타낸다. 
	float	m_fRotationSpeed;
};

class CTurretObject : public CGameObject
{
public:
	CTurretObject();
	virtual ~CTurretObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }

private:
	//포탑 객체의 회전 속력(1초 동안 회전하는 각도)를 나타낸다. 포탑은 y-축으로 360도 회전할 수 있다. 
	float m_fRotationSpeed;
};

class CTankObject : public CGameObject
{
public:
	CTankObject();
	virtual ~CTankObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetMovingSpeed(float fMovingSpeed) { m_fMovingSpeed = fMovingSpeed; }

private:
	//탱크 몸체의 이동 속력(1초 동안 이동하는 거리)을 나타낸다. 
	float m_fMovingSpeed;
};

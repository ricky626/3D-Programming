#pragma once
#include "Mesh.h"
#include "Camera.h"

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
private :
	int m_nReferences;
public :
	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	RECT m_rt;
	int m_nHeight; int m_nWidth;
	void SetRect();
	void SetMaterial(CMaterial *pMaterial);
	virtual void SetMesh(CMesh *pMesh);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera=NULL);

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
};

class CRotatingObject : public CGameObject
{
private:
	//자전 속도와 회전축 벡터를 나타내는 멤버 변수를 선언한다.
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;
public :
	CRotatingObject();
	virtual ~CRotatingObject();
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera = NULL);
	//자전 속도와 회전축 벡터를 설정하는 함수이다.
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }
};
class CCannonBallObject : public CRotatingObject
{
public:
	CCannonBallObject();
	virtual ~CCannonBallObject();

	void SetMovingSpeed(float fMovingSpeed) { m_fMovingSpeed = fMovingSpeed; }

private:
	//포탄 객체의 이동 속력(1초 동안 움직이는 거리)를 나타낸다. 
	float m_fMovingSpeed;
};

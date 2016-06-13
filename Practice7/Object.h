#pragma once

#include "Mesh.h"
#include "Camera.h"

#define DIR_FORWARD	0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08
#define DIR_UP		0x10
#define DIR_DOWN	0x20

extern bool gbAutomaticMoveRotate;

class AABB
{
public:
	//바운딩 박스의 최소점과 최대점을 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	//두 개의 바운딩 박스를 합한다.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	//바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

private:
	int m_nReferences;

public:
	void AddRef();
	void Release();

protected:
	//게임 객체는 메쉬들의 리스트를 갖는다. 
	CMesh **m_ppMeshes;
	int m_nMeshes;
	//게임 객체의 첫 번째 자식 객체에 대한 포인터이다. 
	CGameObject *m_pChild;
	//게임 객체의 형제 객체에 대한 포인터이다. 
	CGameObject *m_pSibling;
	//게임 객체의 부모 객체에 대한 포인터이다. 
	CGameObject *m_pParent;

	//게임 객체가 카메라에 보이는 상태를 나타내는 변수이다.
	bool m_bIsVisible;
	//게임 객체의 활성화 상태를 설정한다. 객체가 비활성화되면 객체는 움직이지 않게 된다.
	bool m_bActive;

	//게임 객체의 로컬 변환 행렬이다. 게임 객체들의 계층구조에서 부모에 상대적인 변환을 나타낸다.
	D3DXMATRIX  m_d3dxmtxTransform;

public:
	//게임 객체의 월드 변환 행렬이다. 게임 객체를 렌더링하려면 월드 변환 행렬이 필요하다.
	D3DXMATRIX  m_d3dxmtxWorld;

public:
	virtual void SetMesh(CMesh *pMesh);
	//게임 객체의 자식 객체를 설정한다. 
	virtual void SetChild(CGameObject *pChild);
	//게임 객체의 부모 객체를 반환한다. 
	CGameObject *GetParent() { return(m_pParent); }

	void SetTransform(D3DXMATRIX d3dxmtxTransform, bool bd3dxmtxTransform = false) { if (bd3dxmtxTransform) m_d3dxmtxTransform = d3dxmtxTransform; else m_d3dxmtxWorld = d3dxmtxTransform; }
	D3DXMATRIX GetTransform(bool bd3dxmtxTransform = false) { return((bd3dxmtxTransform) ? m_d3dxmtxTransform : m_d3dxmtxWorld); }

	void SetPosition(float x, float y, float z, bool bd3dxmtxTransform = false);
	void SetPosition(D3DXVECTOR3 d3dxvPosition, bool bd3dxmtxTransform = false);

	void MoveStrafe(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveUp(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveForward(float fDistance = 1.0f, bool bd3dxmtxTransform = false);

	//게임 객체를 월드 좌표계의 x-축, y-축, z-축 방향으로 이동한다. 
	void MoveStrafeW(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveUpW(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveForwardW(float fDistance = 1.0f, bool bd3dxmtxTransform = false);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f, bool bd3dxmtxTransform = false);
	void Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle, bool bd3dxmtxTransform = false);
	void Revolve(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f, bool bd3dxmtxTransform = false);
	void Revolve(D3DXVECTOR3 *pd3dxvAxis, float fAngle, bool bd3dxmtxTransform = false);

	D3DXVECTOR3 GetPosition(bool bd3dxmtxTransform = false);
	D3DXVECTOR3 GetLook(bool bd3dxmtxTransform = false);
	D3DXVECTOR3 GetUp(bool bd3dxmtxTransform = false);
	D3DXVECTOR3 GetRight(bool bd3dxmtxTransform = false);

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);
	virtual void OnPrepareRender() { }
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);

	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	AABB GetBoundingCube();
	bool IsVisible(CCamera *pCamera = NULL);

	virtual void SetActive(bool bActive = true) { m_bActive = bActive; }
	bool IsActive() { return(m_bActive); }
};


class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);

protected:
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;
};


class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject();
	virtual ~CRevolvingObject();

	virtual void Animate(float fTimeElapsed);

private:
	// 공전 회전축과 회전 속력을 나타낸다.
	D3DXVECTOR3 m_d3dxvRevolutionAxis;
	float m_fRevolutionSpeed;

public:
	// 공전 속력을 설정한다.
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	// 공전을 위한 회전축을 설정한다.
	void SetRevolutionAxis(D3DXVECTOR3 d3dxvRevolutionAxis) { m_d3dxvRevolutionAxis = d3dxvRevolutionAxis; }
};

class CCannonBallObject : public CRotatingObject
{
public:
	CCannonBallObject();
	virtual ~CCannonBallObject();

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);

	void SetMovingSpeed(float fMovingSpeed) { m_fMovingSpeed = fMovingSpeed; }
	virtual void SetActive(bool bActive = true);

private:
	//포탄 객체의 이동 속력(1초 동안 움직이는 거리)를 나타낸다. 
	float m_fMovingSpeed;
	//포탄 객체의 유효사거리를 나타낸다. 
	float m_fEffectiveRange;
	//포탄 객체가 움직인 거리를 나타낸다. 
	float m_fMovingDistance;
};

class CGunObject : public CGameObject
{
public:
	CGunObject(ID3D11Device *pd3dDevice);
	virtual ~CGunObject();

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);

	CCannonBallObject **m_ppBalls;
	int m_nBalls;
	int m_nBallToShoot;

	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetRotationAngle(float fRotationAngle) { m_fRotationAngle = fRotationAngle; }
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }

private:
	/*포신 객체의 현재 회전 각도를 나타낸다. 포신은 위-아래 방향(y-축)으로 정해진 각도 내에서만 회전한다.*/
	float	m_fRotationAngle;
	//포신 객체의 회전 속력(1초 동안 회전하는 각도)를 나타낸다. 
	float	m_fRotationSpeed;
};

class CTurretObject : public CGameObject
{
public:
	CTurretObject();
	virtual ~CTurretObject();

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);

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

	virtual void Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent = NULL);

	void SetMovingSpeed(float fMovingSpeed) { m_fMovingSpeed = fMovingSpeed; }

private:
	//탱크 몸체의 이동 속력(1초 동안 이동하는 거리)을 나타낸다. 
	float m_fMovingSpeed;
};

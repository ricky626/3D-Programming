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
	//�ٿ�� �ڽ��� �ּ����� �ִ����� ��Ÿ���� �����̴�.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	//�� ���� �ٿ�� �ڽ��� ���Ѵ�.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	//�ٿ�� �ڽ��� 8���� �������� ��ķ� ��ȯ�ϰ� �ּ����� �ִ����� �ٽ� ����Ѵ�.
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
	//���� ��ü�� �޽����� ����Ʈ�� ���´�. 
	CMesh **m_ppMeshes;
	int m_nMeshes;
	//���� ��ü�� ù ��° �ڽ� ��ü�� ���� �������̴�. 
	CGameObject *m_pChild;
	//���� ��ü�� ���� ��ü�� ���� �������̴�. 
	CGameObject *m_pSibling;
	//���� ��ü�� �θ� ��ü�� ���� �������̴�. 
	CGameObject *m_pParent;

	//���� ��ü�� ī�޶� ���̴� ���¸� ��Ÿ���� �����̴�.
	bool m_bIsVisible;
	//���� ��ü�� Ȱ��ȭ ���¸� �����Ѵ�. ��ü�� ��Ȱ��ȭ�Ǹ� ��ü�� �������� �ʰ� �ȴ�.
	bool m_bActive;

	//���� ��ü�� ���� ��ȯ ����̴�. ���� ��ü���� ������������ �θ� ������� ��ȯ�� ��Ÿ����.
	D3DXMATRIX  m_d3dxmtxTransform;

public:
	//���� ��ü�� ���� ��ȯ ����̴�. ���� ��ü�� �������Ϸ��� ���� ��ȯ ����� �ʿ��ϴ�.
	D3DXMATRIX  m_d3dxmtxWorld;

public:
	virtual void SetMesh(CMesh *pMesh);
	//���� ��ü�� �ڽ� ��ü�� �����Ѵ�. 
	virtual void SetChild(CGameObject *pChild);
	//���� ��ü�� �θ� ��ü�� ��ȯ�Ѵ�. 
	CGameObject *GetParent() { return(m_pParent); }

	void SetTransform(D3DXMATRIX d3dxmtxTransform, bool bd3dxmtxTransform = false) { if (bd3dxmtxTransform) m_d3dxmtxTransform = d3dxmtxTransform; else m_d3dxmtxWorld = d3dxmtxTransform; }
	D3DXMATRIX GetTransform(bool bd3dxmtxTransform = false) { return((bd3dxmtxTransform) ? m_d3dxmtxTransform : m_d3dxmtxWorld); }

	void SetPosition(float x, float y, float z, bool bd3dxmtxTransform = false);
	void SetPosition(D3DXVECTOR3 d3dxvPosition, bool bd3dxmtxTransform = false);

	void MoveStrafe(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveUp(float fDistance = 1.0f, bool bd3dxmtxTransform = false);
	void MoveForward(float fDistance = 1.0f, bool bd3dxmtxTransform = false);

	//���� ��ü�� ���� ��ǥ���� x-��, y-��, z-�� �������� �̵��Ѵ�. 
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
	// ���� ȸ����� ȸ�� �ӷ��� ��Ÿ����.
	D3DXVECTOR3 m_d3dxvRevolutionAxis;
	float m_fRevolutionSpeed;

public:
	// ���� �ӷ��� �����Ѵ�.
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	// ������ ���� ȸ������ �����Ѵ�.
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
	//��ź ��ü�� �̵� �ӷ�(1�� ���� �����̴� �Ÿ�)�� ��Ÿ����. 
	float m_fMovingSpeed;
	//��ź ��ü�� ��ȿ��Ÿ��� ��Ÿ����. 
	float m_fEffectiveRange;
	//��ź ��ü�� ������ �Ÿ��� ��Ÿ����. 
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
	/*���� ��ü�� ���� ȸ�� ������ ��Ÿ����. ������ ��-�Ʒ� ����(y-��)���� ������ ���� �������� ȸ���Ѵ�.*/
	float	m_fRotationAngle;
	//���� ��ü�� ȸ�� �ӷ�(1�� ���� ȸ���ϴ� ����)�� ��Ÿ����. 
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
	//��ž ��ü�� ȸ�� �ӷ�(1�� ���� ȸ���ϴ� ����)�� ��Ÿ����. ��ž�� y-������ 360�� ȸ���� �� �ִ�. 
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
	//��ũ ��ü�� �̵� �ӷ�(1�� ���� �̵��ϴ� �Ÿ�)�� ��Ÿ����. 
	float m_fMovingSpeed;
};

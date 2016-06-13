#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"


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

	D3DXMATRIX m_d3dxmtxWorld;
	CMesh *m_pMesh;
	CShader *m_pShader;

	D3DXVECTOR3				m_vMovingDirection;
	float					m_fMovingSpeed;

	void SetPosition(float x, float y, float z);
	void SetPosition(D3DXVECTOR3 d3dxvPosition);
	void SetMovingDirection(D3DXVECTOR3 vMovingDirection) { D3DXVec3Normalize(&m_vMovingDirection, &vMovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }

	void Move(D3DXVECTOR3& vDirection, float fSpeed);

	D3DXVECTOR3 GetPosition();

	virtual void SetMesh(CMesh *pMesh);
	virtual void SetShader(CShader *pShader);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};


class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};


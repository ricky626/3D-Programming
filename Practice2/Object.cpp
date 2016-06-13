#include "stdafx.h"
#include "Object.h"


CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pShader = NULL;

	m_nReferences = 0;
}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader) m_pShader->Release();
}

void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}

void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::SetShader(CShader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxWorld._41 = d3dxvPosition.x;
	m_d3dxmtxWorld._42 = d3dxvPosition.y;
	m_d3dxmtxWorld._43 = d3dxvPosition.z;
}

D3DXVECTOR3 CGameObject::GetPosition()
{
	return(D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43));

}

void CGameObject::Move(D3DXVECTOR3& vDirection, float fSpeed)
{
	SetPosition(m_d3dxmtxWorld._41 + vDirection.x * fSpeed,
		m_d3dxmtxWorld._42 + vDirection.y * fSpeed,
		m_d3dxmtxWorld._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate(float fTimeElapsed)
{

}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
		m_pShader->Render(pd3dDeviceContext);
	}
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}


CRotatingObject::CRotatingObject()
{
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	D3DXMATRIX mtxRotate;
	//y-�� ȸ�� ����� �����ϰ� ���� ��ȯ ��Ŀ� ���Ѵ�. 
	D3DXMatrixRotationY(&mtxRotate, (float)D3DXToRadian(45.0f*fTimeElapsed));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;

	if (m_fMovingSpeed != 0.0f)
	{
		D3DXVECTOR3 d3dxvPosition = GetPosition();
		float fDistance = D3DXVec3Length(&d3dxvPosition);
		Move(m_vMovingDirection, m_fMovingSpeed);
	}


}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}
#include "GameObject.h"


CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pMaterial = NULL;
	m_nReferences = 1;
	m_nHeight = m_nWidth = 0;
}

void CGameObject::AddRef()
{
	++m_nReferences;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) --m_nReferences;
	if (m_nReferences <= 0) delete this;
}

void CGameObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void CGameObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::Animate(float fTimeElapsed)
{

}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera)
{
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pMaterial) m_pMaterial->Release();
}

CRotatingObject::CRotatingObject()
{
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{

}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);
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

D3DXVECTOR3 CGameObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return(d3dxvLookAt);
}

D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return(d3dxvRight);
}

void CGameObject::MoveStrafe(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

CMaterial::CMaterial()
{
	m_nReferences = 0;
}

CMaterial::~CMaterial()
{
}

void CMaterial::AddRef()
{
	m_nReferences++;
}

void CMaterial::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences == 0) delete this;
}
void CGameObject::SetRect()
{
	m_rt.left = m_d3dxmtxWorld._41 - m_nWidth / 2;
	m_rt.right = m_rt.left + m_nWidth;
	m_rt.top = m_d3dxmtxWorld._43 - m_nHeight / 2;
	m_rt.bottom = m_rt.top + m_nHeight;
}
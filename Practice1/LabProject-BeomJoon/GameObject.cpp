#include "StdAfx.h"
#include "GameObject.h"

CPolygon::CPolygon(int nVertices)
{
	if (nVertices > 0)
	{
		m_nVertices = nVertices;
		m_pVertices = new CVertex[nVertices];
	}
}

CPolygon::~CPolygon()
{
	if (m_pVertices) delete[] m_pVertices;
}

void CPolygon::SetVertex(int nIndex, CVertex vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon *[nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete[] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CGameObject::CGameObject()
{
	m_pMesh = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);

	m_vMovingDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_fMovingSpeed = 0.0f;
	m_fMovingRange = 0.0f;

	m_vRotationAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.05f;
}

CGameObject::CGameObject(CMesh *pMesh)
{
	m_pMesh = pMesh;
	D3DXMatrixIdentity(&m_mtxWorld);
}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_mtxWorld._41 = x;
	m_mtxWorld._42 = y;
	m_mtxWorld._43 = z;
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_mtxWorld._41 = d3dxvPosition.x;
	m_mtxWorld._42 = d3dxvPosition.y;
	m_mtxWorld._43 = d3dxvPosition.z;
}

D3DXVECTOR3 CGameObject::GetPosition()
{
	return(D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43));
}

D3DXVECTOR3 CGameObject::GetLook()
{
	D3DXVECTOR3 d3dxvLookAt(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return(d3dxvLookAt);
}

D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
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
	D3DXVECTOR3 d3dxvLookAt = GetLook();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate,
		(float)D3DXToRadian(fYaw),
		(float)D3DXToRadian(fPitch),
		(float)D3DXToRadian(fRoll));
	m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3& vRotationAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, &vRotationAxis, (float)D3DXToRadian(fAngle));
	m_mtxWorld = mtxRotate * m_mtxWorld;
}

void CGameObject::Move(D3DXVECTOR3& vDirection, float fSpeed)
{
	SetPosition(m_mtxWorld._41 + vDirection.x * fSpeed,
		m_mtxWorld._42 + vDirection.y * fSpeed,
		m_mtxWorld._43 + vDirection.z * fSpeed);
}

void CGameObject::Animate()
{
	if (m_fRotationSpeed != 0.0f)
		Rotate(m_vRotationAxis, m_fRotationSpeed);

	if (m_fMovingSpeed != 0.0f)
	{
		D3DXVECTOR3 d3dxvPosition = GetPosition();
		float fDistance = D3DXVec3Length(&d3dxvPosition);
		Move(m_vMovingDirection, m_fMovingSpeed);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCamera::CCamera()
{
	m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_d3dxvLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
}

CCamera::~CCamera()
{
}

void CCamera::GenerateViewMatrix()
{
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
	D3DXVec3Cross(&m_d3dxvRight, &m_d3dxvUp, &m_d3dxvLook);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	D3DXVec3Cross(&m_d3dxvUp, &m_d3dxvLook, &m_d3dxvRight);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	m_mtxView._11 = m_d3dxvRight.x; m_mtxView._12 = m_d3dxvUp.x; m_mtxView._13 = m_d3dxvLook.x;
	m_mtxView._21 = m_d3dxvRight.y; m_mtxView._22 = m_d3dxvUp.y; m_mtxView._23 = m_d3dxvLook.y;
	m_mtxView._31 = m_d3dxvRight.z; m_mtxView._32 = m_d3dxvUp.z; m_mtxView._33 = m_d3dxvLook.z;
	m_mtxView._41 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvRight);
	m_mtxView._42 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvUp);
	m_mtxView._43 = -D3DXVec3Dot(&m_d3dxvPosition, &m_d3dxvLook);
}

void CCamera::SetLookAt(D3DXVECTOR3& vPosition, D3DXVECTOR3& d3dxvLookAt, D3DXVECTOR3& d3dxvUp)
{
	m_d3dxvPosition = vPosition;
	D3DXMatrixLookAtLH(&m_mtxView, &m_d3dxvPosition, &d3dxvLookAt, &d3dxvUp);
	m_d3dxvRight = D3DXVECTOR3(m_mtxView._11, m_mtxView._21, m_mtxView._31);
	D3DXVec3Normalize(&m_d3dxvRight, &m_d3dxvRight);
	m_d3dxvUp = D3DXVECTOR3(m_mtxView._12, m_mtxView._22, m_mtxView._32);
	D3DXVec3Normalize(&m_d3dxvUp, &m_d3dxvUp);
	m_d3dxvLook = D3DXVECTOR3(m_mtxView._13, m_mtxView._23, m_mtxView._33);
	D3DXVec3Normalize(&m_d3dxvLook, &m_d3dxvLook);
}

void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight)
{
	m_xViewport = xTopLeft;
	m_yViewport = yTopLeft;
	m_cxViewport = nWidth;
	m_cyViewport = nHeight;
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CCamera::Move(D3DXVECTOR3& d3dxvShift)
{
	m_d3dxvPosition += d3dxvShift;
	GenerateViewMatrix();
}

void CCamera::Move(float x, float y, float z)
{
	Move(D3DXVECTOR3(x, y, z));
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	if (fPitch != 0.0f)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRight, (float)D3DXToRadian(fPitch));
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvUp, (float)D3DXToRadian(fYaw));
		D3DXVec3TransformNormal(&m_d3dxvLook, &m_d3dxvLook, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvLook, (float)D3DXToRadian(fRoll));
		D3DXVec3TransformNormal(&m_d3dxvUp, &m_d3dxvUp, &mtxRotate);
		D3DXVec3TransformNormal(&m_d3dxvRight, &m_d3dxvRight, &mtxRotate);
	}

	GenerateViewMatrix();
}


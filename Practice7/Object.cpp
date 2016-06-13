
#include "stdafx.h"
#include "Object.h"

//��A' Ű�� ������ �� ������ ���� true�� �ȴ�. �ٽ� �ѹ� 'A' Ű�� ������ false�� �ȴ�.
bool gbAutomaticMoveRotate = false;

CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxTransform);
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_ppMeshes = NULL;
	m_nMeshes = 0;
	m_pChild = m_pSibling = m_pParent = NULL;
	m_bIsVisible = false;
	m_bActive = true;
	m_nReferences = 0;
}

CGameObject::~CGameObject()
{
	for (int i = 0; i < m_nMeshes; i++) if (m_ppMeshes[i]) m_ppMeshes[i]->Release();
	if (m_ppMeshes) delete[] m_ppMeshes;
	if (m_pSibling) delete m_pSibling;
	if (m_pChild) delete m_pChild;
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
	if (pMesh)
	{
		CMesh **ppMeshes = new CMesh*[m_nMeshes + 1];
		for (int i = 0; i < m_nMeshes; i++) ppMeshes[i] = m_ppMeshes[i];
		ppMeshes[m_nMeshes] = pMesh;
		delete[] m_ppMeshes;
		m_ppMeshes = ppMeshes;
		m_nMeshes++;
		pMesh->AddRef();
	}
}

void CGameObject::SetChild(CGameObject *pChild)
{
	//��ü�� ù ��° �ڽ� ��ü�� �����ϸ� pChild�� m_pChild ��ü�� ������ ���� ��ü�� �����Ѵ�.
	if (m_pChild)
	{
		if (pChild) pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}
	//pChild ��ü�� �θ� ��ü�� ������ ��ü�� �����Ѵ�.
	if (pChild) pChild->m_pParent = this;
}

/*���� ��ü�� ��ġ�� ���� ��ǥ�� �Ǵ� ���� ��ǥ�踦 ����Ͽ� �����Ѵ�. bd3dxmtxTransform�� ���̸� ���� ��ü���� ������������ �θ� ������� ��ġ�� �����Ѵ�.*/
void CGameObject::SetPosition(float x, float y, float z, bool bd3dxmtxTransform)
{
	if (bd3dxmtxTransform)
	{
		m_d3dxmtxTransform._41 = x;
		m_d3dxmtxTransform._42 = y;
		m_d3dxmtxTransform._43 = z;
	}
	else
	{
		m_d3dxmtxWorld._41 = x;
		m_d3dxmtxWorld._42 = y;
		m_d3dxmtxWorld._43 = z;
	}
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition, bool bd3dxmtxTransform)
{
	SetPosition(d3dxvPosition.x, d3dxvPosition.y, d3dxvPosition.z, bd3dxmtxTransform);
}

void CGameObject::MoveStrafe(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	//���� ��ü�� ���� ��ǥ���� x-�� �������� �����δ�.
	D3DXVECTOR3 d3dxvRight = GetRight(bd3dxmtxTransform);
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

void CGameObject::MoveUp(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	D3DXVECTOR3 d3dxvUp = GetUp(bd3dxmtxTransform);
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

void CGameObject::MoveForward(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	D3DXVECTOR3 d3dxvLookAt = GetLook(bd3dxmtxTransform);
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

void CGameObject::MoveStrafeW(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	//���� ��ǥ���� x-�� �������� �����δ�.
	D3DXVECTOR3 d3dxvRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

void CGameObject::MoveUpW(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	D3DXVECTOR3 d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

void CGameObject::MoveForwardW(float fDistance, bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition(bd3dxmtxTransform);
	D3DXVECTOR3 d3dxvLookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition, bd3dxmtxTransform);
}

D3DXVECTOR3 CGameObject::GetPosition(bool bd3dxmtxTransform)
{
	return((bd3dxmtxTransform) ? D3DXVECTOR3(m_d3dxmtxTransform._41, m_d3dxmtxTransform._42, m_d3dxmtxTransform._43) : D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43));
}

D3DXVECTOR3 CGameObject::GetLook(bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvLookAt = (bd3dxmtxTransform) ? D3DXVECTOR3(m_d3dxmtxTransform._31, m_d3dxmtxTransform._32, m_d3dxmtxTransform._33) : D3DXVECTOR3(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return(d3dxvLookAt);
}

D3DXVECTOR3 CGameObject::GetUp(bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvUp = (bd3dxmtxTransform) ? D3DXVECTOR3(m_d3dxmtxTransform._21, m_d3dxmtxTransform._22, m_d3dxmtxTransform._23) : D3DXVECTOR3(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight(bool bd3dxmtxTransform)
{
	D3DXVECTOR3 d3dxvRight = (bd3dxmtxTransform) ? D3DXVECTOR3(m_d3dxmtxTransform._11, m_d3dxmtxTransform._12, m_d3dxmtxTransform._13) : D3DXVECTOR3(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return(d3dxvRight);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll, bool bd3dxmtxTransform)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	if (bd3dxmtxTransform)
		m_d3dxmtxTransform = mtxRotate * m_d3dxmtxTransform;
	else
		m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle, bool bd3dxmtxTransform)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	if (bd3dxmtxTransform)
		m_d3dxmtxTransform = mtxRotate * m_d3dxmtxTransform;
	else
		m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Revolve(float fPitch, float fYaw, float fRoll, bool bd3dxmtxTransform)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	if (bd3dxmtxTransform)
		m_d3dxmtxTransform = m_d3dxmtxTransform * mtxRotate;
	else
		m_d3dxmtxWorld = m_d3dxmtxWorld * mtxRotate;
}

void CGameObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	/*�θ� ��ü�� ������ �� �θ� ��ü�� ���� ��ȯ ����� ���� ��ü�� ���� ��ȯ ��Ŀ� ���Ͽ� ���� ��ȯ ��Ŀ� �����Ѵ�.*/
	if (pd3dxmtxParent) D3DXMatrixMultiply(&m_d3dxmtxWorld, &m_d3dxmtxTransform, pd3dxmtxParent);

	/*��ü�� ���� ��ü�� �ִϸ��̼��Ѵ�. ���� ��ü�� �ִϸ��̼��� �θ� ��ü�� ���� ��ȯ ����� ����Ѵ�.*/
	if (m_pSibling) m_pSibling->Animate(fTimeElapsed, pd3dxmtxParent);
	/*��ü�� ù ��° �ڽ� ��ü�� �ִϸ��̼��Ѵ�. �ڽ� ��ü�� �ִϸ��̼��� ���� ��ü�� ���� ��ȯ ����� ����Ѵ�.*/
	if (m_pChild) m_pChild->Animate(fTimeElapsed, &m_d3dxmtxWorld);
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	/*���� ��ü�� ���δٸ� �޽��� �������Ѵ�. m_bIsVisible ������ ���� CGameObject::IsVisible() �Լ����� �����ȴ�.*/
	if (m_bIsVisible)
	{
		OnPrepareRender();
		//���� ��ȯ ����� �����Ͽ� ���̴��� ��� ���� ������ �����Ѵ�.
		CShader::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
		//���� ���� ��ü�� ��� �޽��� �������Ѵ�.
		for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i]->Render(pd3dDeviceContext);
	}

	//���� ��ü�� ���� ��ü�� �������Ѵ�. 
	if (m_pSibling) m_pSibling->Render(pd3dDeviceContext, pCamera);
	//���� ��ü�� ù ��° �ڽ� ��ü�� �������Ѵ�. 
	if (m_pChild) m_pChild->Render(pd3dDeviceContext, pCamera);
}

//���� ��ü(���� ����)�� Ű���� �޽����� ó���� �� �ֵ��� �Ѵ�.
bool CGameObject::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	bool bChild = false, bSibling = false;
	if (m_pSibling) bSibling = m_pSibling->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	if (m_pChild) bChild = m_pChild->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
	return(bChild | bSibling);
}

CRotatingObject::CRotatingObject()
{
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	CGameObject::Rotate(&m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);
}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);
}

CRevolvingObject::CRevolvingObject()
{
	m_d3dxvRevolutionAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_fRevolutionSpeed = 0.0f;
}

CRevolvingObject::~CRevolvingObject()
{
}

void CRevolvingObject::Animate(float fTimeElapsed)
{
	//������ ��Ÿ���� ���� ȸ�� ����� �����ʿ� ���Ѵ�.
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRevolutionAxis, (float)D3DXToRadian(m_fRevolutionSpeed * fTimeElapsed));
	m_d3dxmtxWorld = m_d3dxmtxWorld * mtxRotate;
}

/*
AABB CGameObject::GetBoundingCube()
{
	//��� �޽��� �ٿ�� �ڽ��� ���Ͽ� ���� ��ȯ ��ķ� ��ȯ�Ѵ�.
	AABB bcBoundingCube = m_ppMeshes[0]->GetBoundingCube();
	for (int i = 1; i < m_nMeshes; i++) bcBoundingCube.Union(&m_ppMeshes[i]->GetBoundingCube());
	bcBoundingCube.Transform(&m_d3dxmtxWorld);
	return(bcBoundingCube);
}

bool CGameObject::IsVisible(CCamera *pCamera)
{
	if (!pCamera) return(m_bIsVisible = true);

	bool bIsVisible = false
		//���� ���� ��ü�� �ٿ�� �ڽ��� ī�޶��� ����ü�� ��ġ�� ���� �˻��Ѵ�.
		AABB bcBoundingCube = GetBoundingCube();
	bIsVisible = m_bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);

	//���� ���� ��ü�� ���� ��ü�� ī�޶� ���̴� ���� �˻��Ѵ�.
	if (m_pSibling) bIsVisible |= m_pSibling->IsVisible(pCamera);
	//���� ���� ��ü�� �ڽ� ��ü�� ī�޶� ���̴� ���� �˻��Ѵ�.
	if (m_pChild) bIsVisible |= m_pChild->IsVisible(pCamera);

	//���� ���� ��ü�� ���� ���� �� �ϳ��� ī�޶� ���̸� �� ��ü�� ī�޶� ���̴� ���̴�.
	return(bIsVisible);
}
*/
CCannonBallObject::CCannonBallObject()
{
	m_fRotationSpeed = 90.0f;
	m_d3dxvRotationAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_fMovingSpeed = 70.0f;
	m_fEffectiveRange = m_fMovingSpeed * 5.0f;
	m_fMovingDistance = 0.0f;

	//��ź�� �ʱ⿡ ��Ȱ��ȭ�ȴ�.
	m_bActive = false;
}

CCannonBallObject::~CCannonBallObject()
{
}

void CCannonBallObject::SetActive(bool bActive)
{
	m_fMovingDistance = 0.0f;
	m_bActive = bActive;
}

void CCannonBallObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	CRotatingObject::Animate(fTimeElapsed, pd3dxmtxParent);
	//��ź�� �̵� �ӷ¿� ���� �����̰� ������ �Ÿ��� ��ü �̵� �Ÿ��� ���Ѵ�.
	MoveStrafe(m_fMovingSpeed * fTimeElapsed, false);
	m_fMovingDistance += m_fMovingSpeed * fTimeElapsed;
	//��ź�� ��ü �̵� �Ÿ��� ��ȿ ��Ÿ� ���� ũ�� ��ź�� ��Ȱ��ȭ�Ѵ�.
	if (m_fMovingDistance > m_fEffectiveRange) SetActive(false);
}

CGunObject::CGunObject(ID3D11Device *pd3dDevice)
{
	m_fRotationAngle = 0.0f;
	m_fRotationSpeed = 30.0f;

	CSphereMesh *pBallMesh = new CSphereMesh(pd3dDevice, 2.5f, 10, 10, D3DCOLOR_XRGB(0, 0, 128));

	m_nBalls = 10;
	m_ppBalls = new CCannonBallObject*[m_nBalls];
	for (int i = 0; i < m_nBalls; i++)
	{
		m_ppBalls[i] = new CCannonBallObject();
		m_ppBalls[i]->SetMesh(pBallMesh);
		m_ppBalls[i]->AddRef();
	}
}

CGunObject::~CGunObject()
{
	for (int i = 0; i < m_nBalls; i++) if (m_ppBalls[i]) m_ppBalls[i]->Release();
	if (m_ppBalls) delete[] m_ppBalls;
}

void CGunObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	/*Ű������ 'A' Ű�� �������� ������ ���� 45��, �Ʒ��� 10������ ��� ȸ���Ѵ�. �� �Ʒ��� ȸ���� z-���� �߽����� ȸ���ϴ� ���̴�. �̰��� ������ �� �޽��� �������� ����� ������ ������ x-������ �����Ͽ��� �����̴�.*/
	if (::gbAutomaticMoveRotate)
	{
		if ((m_fRotationAngle <= -10.0f) || (m_fRotationAngle >= 45.0f)) m_fRotationSpeed = -m_fRotationSpeed;
		float fRotateAngle = m_fRotationSpeed * fTimeElapsed;
		m_fRotationAngle += fRotateAngle;
		Rotate(0.0f, 0.0f, fRotateAngle, true);
	}

	CGameObject::Animate(fTimeElapsed, pd3dxmtxParent);

	//��ź�� Ȱ��ȭ�Ǹ� ��ź�� �ִϸ��̼��Ѵ�.
	for (int i = 0; i < m_nBalls; i++)
	{
		if (m_ppBalls[i]->IsActive()) m_ppBalls[i]->Animate(fTimeElapsed, NULL);
	}
}

void CGunObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext, pCamera);

	//��ź�� Ȱ��ȭ�ǰ� ī�޶� ���δٸ� ��ź�� �������Ѵ�.
	for (int i = 0; i < m_nBalls; i++)
	{
		if (m_ppBalls[i]->IsActive() && m_ppBalls[i]->IsVisible(pCamera)) m_ppBalls[i]->Render(pd3dDeviceContext, pCamera);
	}
}

//��ź�� �߻��ϱ� ���Ͽ� Ű���� �޽����� ���� ��ü���� ó���Ѵ�.
bool CGunObject::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
			//��Space' Ű�� �������� ��ź�� ���� ������ �������� �߻��Ѵ�.
		case VK_SPACE: //Fire 
		{
			int nBallToShoot = m_nBalls;
			for (int i = 0; i < m_nBalls; i++)
			{
				if (!m_ppBalls[i]->IsActive())
				{
					nBallToShoot = i;
					break;
				}
			}
			if (nBallToShoot < m_nBalls)
			{
				//����� �� �ִ� ��ź ��ü�� ������ �� ��ź ��ü�� Ȱ��ȭ�Ѵ�.
				m_ppBalls[nBallToShoot]->SetActive(true);
				/*��ź ��ü�� ���� ��ȯ ����� ���� ��ü�� ���� ��ȯ ��ķ� �����Ѵ�. �׷��� ��ź�� ��ġ�� ������ ���� ��ü�� ��ġ�� ����� ��������. ����(����) ���� ������ x-�� �����̹Ƿ� ��ź�� �߻��ϱ� ���Ͽ� ��ź�� ���� x-�� �������� �����̸� �ȴ�.*/
				m_ppBalls[nBallToShoot]->SetTransform(m_d3dxmtxWorld, false);
			}
			return(true);
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}
	return(false);
}

CTurretObject::CTurretObject()
{
	m_fRotationSpeed = 45.0f;
}

CTurretObject::~CTurretObject()
{
}

void CTurretObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	//Ű������ 'A' Ű�� �������� ��ž�� y-���� �߽����� ȸ���Ѵ�.
	if (::gbAutomaticMoveRotate) Rotate(0.0f, m_fRotationSpeed * fTimeElapsed, 0.0f, true);
	CGameObject::Animate(fTimeElapsed, pd3dxmtxParent);
}

CTankObject::CTankObject()
{
	m_fMovingSpeed = 1.0f;
}

CTankObject::~CTankObject()
{
}

void CTankObject::Animate(float fTimeElapsed, D3DXMATRIX *pd3dxmtxParent)
{
	//Ű������ 'A' Ű�� �������� ��ũ�� ���� y-���� �߽����� ȸ���ϸ鼭 ���� ��ǥ���� z-�� �������� �ݺ������� ��-�ڷ� �̵��Ѵ�.
	if (::gbAutomaticMoveRotate)
	{
		static float sfBodyMovement = 0.0f, sfMovement = 5.0f;
		if ((sfBodyMovement >= 80.0f) || (sfBodyMovement <= -30.0f)) sfMovement = -sfMovement;
		float fMovement = sfMovement * fTimeElapsed;
		sfBodyMovement += fMovement;
		MoveForwardW(fMovement, false);
		Rotate(0.0f, 15.0f * fTimeElapsed, 0.0f, false);
	}

	CGameObject::Animate(fTimeElapsed, pd3dxmtxParent);
}
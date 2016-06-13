#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
	m_ppObjects = NULL;
	m_nObjects = 0;
}
CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	CShader *pShader = new CShader();
	pShader->CreateShader(pd3dDevice);

	//����x����x���̰� 12x12x12�� ������ü �޽��� �����Ѵ�(�⺻ ������ �Ķ���).
	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DCOLOR_XRGB(0, 0, 128));

	int xObjects = 5, yObjects = 5, zObjects = 5, i = 0;
	/*<�׸� 21>�� ���� x-��, y-��, z-������ 11���� �� 11 x 11 x 11 = 1,331���� ������ü�� ��ġ�Ѵ�. x-��, y-��, z-�࿡�� ������ �������� 5��, ���������� 5��, �� ���� 1�� ��ġ�Ѵ�.*/
	m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1);
	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 2.5f;
	float fyPitch = 12.0f * 2.5f;
	float fzPitch = 12.0f * 2.5f;
	//������ü ��ü�� �����Ѵ�.
	CRotatingObject *pRotatingObject = NULL;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int z = -zObjects; z <= zObjects; z++)
			{
				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pCubeMesh);
				pRotatingObject->SetShader(pShader);
				//�� ������ü ��ü�� ��ġ�� �����Ѵ�.
				pRotatingObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				//�� ������ü ��ü�� ȸ������ y-������ �����Ѵ�.
				pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(30.0f + 10.0f*(i % 10));
				m_ppObjects[i++] = pRotatingObject;
			}
		}
	}
}

void CScene::ReleaseObjects()
{
	//���� ��ü ����Ʈ�� �� ��ü�� ��ȯ(Release)�ϰ� ����Ʈ�� �Ҹ��Ų��.
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}


bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput()
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	//��� ���� ��ü���� �������Ѵ�.
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Animate(fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext)
{
	//��� ���� ��ü���� �������Ѵ�.
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Render(pd3dDeviceContext);
	}
}
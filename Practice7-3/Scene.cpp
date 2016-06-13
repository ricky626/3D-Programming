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

	//����x����x���̰� 12x12x12�� ������ü �޽��� �����Ѵ�.
	CCubeMesh *pMesh = new CCubeMesh(pd3dDevice, 15.0f, 15.0f, 15.0f);

	/*x-��, y-��, z-�� ���� ������ ��ü �����̴�. �� ���� 1�� �÷����鼭 ������ �� ������ ����Ʈ�� ��� ���ϴ� ���� ���캸�� �ٶ���.*/
	int xObjects = 2, yObjects = 2, zObjects = 2;
	//<�׸� 2>�� ���� x-��, y-��, z-������ 7���� �� 7 x 7 x 7 = 343���� ������ü�� ��ġ�Ѵ�. 
	m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1);
	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 2.5f;
	float fyPitch = 12.0f * 2.5f;
	float fzPitch = 12.0f * 2.5f;
	CRotatingObject *pObject = NULL;
	for (int i = 0, z = +zObjects; z >= -zObjects; z--)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int x = -xObjects; x <= xObjects; x++)
			{
				pObject = new CRotatingObject();
				pObject->SetMesh(pMesh);
				pObject->SetShader(pShader);
				//�� ������ü ��ü�� ��ġ�� �����Ѵ�.
				pObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				m_ppObjects[i++] = pObject;
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
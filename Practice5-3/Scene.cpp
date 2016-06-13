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
	//���̴� ��ü�� �����Ѵ�.
	CShader *pShader = new CShader();
	pShader->CreateShader(pd3dDevice);

	//���� ��ü�� ���� �����͵��� �迭�� �����Ѵ�.
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	//�ﰢ�� �޽�(CTriangleMesh)�� �����Ѵ�.
	CTriangleMesh *pMesh = new CTriangleMesh(pd3dDevice);

	//�ﰢ�� ��ü(CRotatingObject)�� �����ϰ� �ﰢ�� �޽��� ���̴��� �����Ѵ�.
	CRotatingObject *pObject = new CRotatingObject();
	pObject->SetMesh(pMesh);
	pObject->SetShader(pShader);

	//��ü�� �����Ѵ�.
	m_ppObjects[0] = pObject;
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
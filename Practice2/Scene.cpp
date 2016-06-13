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

	m_nObjects = 51;
	m_ppObjects = new CGameObject*[m_nObjects];
	//가로x세로x높이가 15x15x15인 정육면체 메쉬를 생성한다.
	CCubeMesh *pMesh = new CCubeMesh(pd3dDevice, 50.0f, 50.0f, 50.0f); // random color cube draw
	CRotatingObject *pObject = new CRotatingObject();
	//pObject->SetPosition(0, 0, 0);
	pObject->SetMesh(pMesh);
	pObject->SetShader(pShader);
	m_ppObjects[0] = pObject;


	for (int i = 1; i <= 50; ++i)
	{
		CCubeMesh *pMesh = new CCubeMesh(pd3dDevice, 5.0f, 5.0f, 5.0f); // random color cube draw
		CRotatingObject *pObject = new CRotatingObject();
		pObject->SetPosition(-10.0f, 0, 0);
		pObject->SetMovingDirection(D3DXVECTOR3(float(rand() % 10) / 100.f, float(rand() % 10) / 100.f, float(rand() % 10) / 100.f));
		pObject->SetMovingSpeed(0.03f);

		pObject->SetMesh(pMesh);
		pObject->SetShader(pShader);
		
		m_ppObjects[i] = pObject;
	}
}

void CScene::ReleaseObjects()
{
	//게임 객체 리스트의 각 객체를 반환(Release)하고 리스트를 소멸시킨다.
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) m_ppObjects[j]->Release();
		delete[] m_ppObjects;
	}
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch(nMessageID)
	{
	case WM_LBUTTONDOWN:
		
		for (int i = 1; i < m_nObjects; ++i)
		{
			m_ppObjects[i]->SetMovingDirection(D3DXVECTOR3(0, 0, 0));
		}
		break;
		//return(true);
	}
	//return(false);
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//return(false);
}

void CScene::ProcessInput()
{
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nObjects; ++i)
	{
		if (i == 0)
			continue;
		m_ppObjects[i]->Animate(fTimeElapsed);

	}

	for (int i = 1; i < m_nObjects; ++i)
	{
		if (m_ppObjects[i]->GetPosition().x > planeX || m_ppObjects[i]->GetPosition().x < -planeX || m_ppObjects[i]->GetPosition().y > planeY ||
			m_ppObjects[i]->GetPosition().y < -planeY || m_ppObjects[i]->GetPosition().z > planeZ+30 || m_ppObjects[i]->GetPosition().z < -planeZ)
		{
			m_ppObjects[i]->SetMovingDirection((D3DXVECTOR3(-m_ppObjects[i]->m_vMovingDirection.x, -m_ppObjects[i]->m_vMovingDirection.y, -m_ppObjects[i]->m_vMovingDirection.z)));
			
		}
			
	}

}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Render(pd3dDeviceContext);
	}
}


//void CScene::CollisionOutWall(CGameObject& cube, const CGameObject& wall)
//{
//	D3DXPLANE plane[6];
//	GetPlaneFromPoint(plane, wall);
//
//	D3DXVECTOR3 vec3tmp;
//	D3DXVECTOR4 vec4tmp;
//
//	for (int i = 0; i < 6; ++i)
//	{
//		GetVectorFromVertex(&vec3tmp, cube.m_pMesh->m_ppPolygons[i]->m_pVertices[0]);
//		
//		D3DXVec3Transform(&vec4tmp, &vec3tmp, &cube.m_d3dxmtxWorld);
//
//		if (D3DXPlaneDot(&plane[i], &vec4tmp) >= 0)
//		{
//			D3DXVECTOR3 r;
//
//			GetReflexVector(&r, cube.m_vMovingDirection, plane[i]);
//
//			cube.SetMovingDirection(r);
//		}
//	}
//}
//
//bool CScene::CrashDectection(CGameObject& cube, const CGameObject& wall)
//{
//	D3DXPLANE plane[6];
//	GetPlaneFromPoint(plane, wall);
//
//	D3DXVECTOR3 vec3tmp;
//	D3DXVECTOR4 vec4tmp;
//
//	for (int i = 0; i < 6; ++i)
//	{
//		GetVectorFromVertex(&vec3tmp, cube.m_pMesh->m_ppPolygons[i]->m_pVertices[0]);
//
//		D3DXVec3Transform(&vec4tmp, &vec3tmp, &cube.m_d3dxmtxWorld);
//
//		if (D3DXPlaneDot(&plane[i], &vec4tmp) >= 0)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//bool CScene::CollisionInnerWall(CGameObject& cube, CGameObject& wall)
//{
//	float fDistance;
//	D3DXVECTOR3 vDiff;
//
//
//	vDiff.x = wall.GetPosition().x - cube.GetPosition().x;
//	vDiff.y = wall.GetPosition().y - cube.GetPosition().y;
//	vDiff.z = wall.GetPosition().z - cube.GetPosition().z;
//
//	fDistance = D3DXVec3Length(&vDiff);
//
//	if (fDistance <= (2))
//		return true;
//	return false;
//}
//
//void CScene::GetPlaneFromPoint(D3DXPLANE* out, const CGameObject& obj)
//{
//	D3DXVECTOR3 point[3];
//
//	for (int i = 0; i < 6; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			point[j].x = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].GetPos().x;
//			
//			point[j].y = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].GetPos().y;
//			point[j].z = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].GetPos().z;
//		}
//		D3DXPlaneFromPoints(&out[i], &point[0], &point[1], &point[2]);
//	}
//}
//
//void CScene::GetVectorFromVertex(D3DXVECTOR3* out, const CVertex& vertex)
//{
//	out->x = vertex.GetPos().x;
//	out->y = vertex.GetPos().y;
//	out->z = vertex.GetPos().z;
//}
//
//void CScene::GetReflexVector(D3DXVECTOR3* out, const D3DXVECTOR3& dir, const D3DXPLANE& plane)
//{
//	D3DXVECTOR3 n(plane.a, plane.b, plane.c);
//
//	*out = dir - 2 * D3DXVec3Dot(&dir, &n) * n;
//}
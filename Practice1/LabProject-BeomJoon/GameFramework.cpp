//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_hInstance = NULL;
	m_hWnd = NULL;
	m_hMenu = NULL;

	m_hDCFrameBuffer = NULL;
	m_hBitmapFrameBuffer = NULL;
	m_hBitmapSelect = NULL;

	m_cxViewport = CLIENT_WIDTH;
	m_cyViewport = CLIENT_HEIGHT;

	m_bActive = true;

	m_nObjects = 0;
	m_bBulletCheck = false;
}

CGameFramework::~CGameFramework()
{
}

bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_xViewport = rc.left;
	m_yViewport = rc.top;
	m_cxViewport = rc.right - rc.left;
	m_cyViewport = rc.bottom - rc.top;

	BuildFrameBuffer();

	srand(timeGetTime());

	BuildObjects();

	SetupGameState();

	return(true);
}

void CGameFramework::BuildFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);

	if (!m_hDCFrameBuffer) m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	if (m_hBitmapFrameBuffer)
	{
		::SelectObject(m_hDCFrameBuffer, NULL);
		::DeleteObject(m_hBitmapFrameBuffer);
		m_hBitmapFrameBuffer = NULL;
	}
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_cxViewport, m_cyViewport);
	::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HBRUSH hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_xViewport, m_yViewport, m_cxViewport, m_cyViewport);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{
	HDC hDC = ::GetDC(m_hWnd);
	::BitBlt(hDC, m_xViewport, m_yViewport, m_cxViewport, m_cyViewport, m_hDCFrameBuffer, m_xViewport, m_yViewport, SRCCOPY);
	::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::ResetDisplay(int cxViewport, int cyViewport)
{
	m_cxViewport = cxViewport;
	m_cyViewport = cyViewport;

	BuildFrameBuffer();
}

void CGameFramework::SetupGameState()
{
}

void CGameFramework::SetupRenderStates()
{
}

CMesh *CGameFramework::BuildCubeMesh(float size, UINT rcolor, UINT gcolor, UINT bcolor)
{
	CMesh *pMesh = new CMesh(6);

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pFrontFace->SetVertex(1, CVertex(+size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pFrontFace->SetVertex(2, CVertex(+size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pFrontFace->SetVertex(3, CVertex(-size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pTopFace->SetVertex(1, CVertex(+size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pTopFace->SetVertex(2, CVertex(+size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pTopFace->SetVertex(3, CVertex(-size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pBackFace->SetVertex(1, CVertex(+size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pBackFace->SetVertex(2, CVertex(+size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pBackFace->SetVertex(3, CVertex(-size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pBottomFace->SetVertex(1, CVertex(+size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pBottomFace->SetVertex(2, CVertex(+size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pBottomFace->SetVertex(3, CVertex(-size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pLeftFace->SetVertex(1, CVertex(-size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pLeftFace->SetVertex(2, CVertex(-size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pLeftFace->SetVertex(3, CVertex(-size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+size, +size, -size, RGB(rcolor, gcolor, bcolor)));
	pRightFace->SetVertex(1, CVertex(+size, +size, +size, RGB(rcolor, gcolor, bcolor)));
	pRightFace->SetVertex(2, CVertex(+size, -size, +size, RGB(rcolor, gcolor, bcolor)));
	pRightFace->SetVertex(3, CVertex(+size, -size, -size, RGB(rcolor, gcolor, bcolor)));
	pMesh->SetPolygon(5, pRightFace);

	return(pMesh);
}


void CGameFramework::BuildObjects()
{
	// 큰 정육면체 생성 //
	CMesh* pOutMesh = BuildCubeMesh(10, 0, 0, 0);

	m_pOutWall = new CGameObject;
	m_pOutWall->SetMesh(pOutMesh);
	m_pOutWall->SetPosition(0.f, 0.f, 0.f);

	m_pOutWall->SetMovingDirection(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pOutWall->SetMovingSpeed(0.f);
	m_pOutWall->SetMovingRange(0.f);

	m_pOutWall->SetRotationAxis(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pOutWall->SetRotationSpeed(0.0f);

	// 탱크 생성 ///
	CMesh* pTank = BuildCubeMesh(0.8f, 255, 0, 0);

	m_pTank = new CGameObject;
	m_pTank->SetMesh(pTank);
	m_pTank->SetPosition(0.f, 0.f, -6.f);

	m_pTank->SetMovingDirection(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pTank->SetMovingSpeed(0.f);
	m_pTank->SetMovingRange(0.f);

	m_pTank->SetRotationAxis(D3DXVECTOR3(0.f, 1.f, 0.f));
	m_pTank->SetRotationSpeed(0.1f);


	//--------------------------------------------------------------//


	// 총알 생성 //
	CMesh* pBullet = BuildCubeMesh(0.5f, 0, 255, 0);

	m_pBullet = new CGameObject;
	m_pBullet->SetMesh(pBullet);
	m_pBullet->SetPosition(0.f, 0.f, 0.f);

	m_pBullet->SetMovingDirection(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pBullet->SetMovingSpeed(0.f);
	m_pBullet->SetMovingRange(0.f);

	m_pBullet->SetRotationAxis(D3DXVECTOR3(0.f, 0.f, 0.f));
	//m_pBullet->SetRotationSpeed(0.1f);


	// 작은 정육면체 생성 //
	CMesh* pMesh = BuildCubeMesh(0.5, 0, 0, 0);

	m_nObjects = 40;
	m_pObjects = new CGameObject[m_nObjects];

	for (int i = 0; i < m_nObjects; ++i)
	{
		m_pObjects[i].SetMesh(pMesh);
		m_pObjects[i].SetPosition(5, 0, 0);
		m_pObjects[i].SetRotationAxis(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_pObjects[i].SetRotationSpeed(0.0f);

		m_pObjects[i].SetMovingDirection(D3DXVECTOR3(float(rand() % 10) / 100.f, float(rand() % 10) / 100.f, float(rand() % 10) / 100.f));
		m_pObjects[i].SetMovingSpeed(0.02f);
		m_pObjects[i].SetMovingRange(100.f);

		m_pObjects[i].SetRotationAxis(D3DXVECTOR3(0, 1, 0));
		m_pObjects[i].SetRotationSpeed(1.0f);
		m_pObjects[i].m_flag = 0;

	}

	// 카메라 생성
	m_pCamera = new CCamera();
	m_pCamera->SetViewport(m_xViewport, m_yViewport, m_cxViewport, m_cyViewport);
	m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, (float(m_cxViewport) / float(m_cyViewport)), 60.0f);
	m_pCamera->SetLookAt(D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

void CGameFramework::ReleaseObjects()
{
	if (m_pObjects) delete[] m_pObjects;
	m_pObjects = NULL;

	if (m_pOutWall) delete m_pOutWall;
	m_pOutWall = nullptr;

	if (m_pTank) delete m_pTank;
	m_pTank = nullptr;

	if (m_pBullet) delete m_pBullet;
	m_pBullet = nullptr;

}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);

	if (m_hMenu) DestroyMenu(m_hMenu);
	m_hMenu = NULL;

	SetMenu(m_hWnd, NULL);
	if (m_hWnd) DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}

void CGameFramework::ProcessInput()
{

	if (GetKeyState(VK_LEFT) & 0xFF00) m_pTank->Move(D3DXVECTOR3(-1, 0, 0), 0.01f);
	if (GetKeyState(VK_RIGHT) & 0xFF00) m_pTank->Move(D3DXVECTOR3(1, 0, 0), 0.01f);
	if (GetKeyState(VK_UP) & 0xFF00) m_pTank->Move(D3DXVECTOR3(0, 1, 0), 0.01f);
	if (GetKeyState(VK_DOWN) & 0xFF00) m_pTank->Move(D3DXVECTOR3(0, -1, 0), 0.01f);

	if (GetKeyState(0x46) & 0xFF00) // key F
	{
		if (!m_bBulletCheck)
			m_bBulletCheck = true;
	}

}

void CGameFramework::AnimateObjects()
{

	D3DXVECTOR3 oldPosition;


	for (int i = 0; i < m_nObjects; ++i)
	{

		CollisionOutWall(m_pObjects[i], *m_pOutWall);

		oldPosition.x = m_pObjects[i].GetPosition().x;
		oldPosition.y = m_pObjects[i].GetPosition().y;
		oldPosition.z = m_pObjects[i].GetPosition().z;

		m_pObjects[i].Animate();
	}

	// 자동으로 은면제거가 되는것인가? // 오브젝트를 안그리게 되면 프레임레이트가 올라간다.
	//for (int i = 0; i < m_nObjects; ++i)
	//{
	//	if (CollisionInnerWall(*m_pBullet, m_pObjects[i]) && m_bBulletCheck)
	//	{
	//		m_pObjects[i].m_flag = 1;
	//		continue;
	//	}
	//}

	for (int i = 0; i < m_nObjects; ++i)
	{
		if (CollisionInnerWall(*m_pBullet, m_pObjects[i]) && m_bBulletCheck)
		{
			m_pObjects[i].SetPosition(-20, 10, 0);
			m_pObjects[i].SetMovingSpeed(0.0f);
			m_pObjects[i].SetRotationSpeed(0.0f);
			continue;
		}
	}

	m_pTank->Animate();

	if (m_bBulletCheck == true)
	{
		if (!CrashDectection(*m_pBullet, *m_pOutWall))
			m_pBullet->Move(D3DXVECTOR3(0, 0, 1), 0.05f);

		else
			m_bBulletCheck = false;
	}
	else
		m_pBullet->SetPosition(m_pTank->GetPosition());


}


void CGameFramework::DrawPrimitive(CPolygon *pPolygon, D3DXMATRIX& mtxTransform)
{
	CVertex vertex;
	D3DXVECTOR3 vPrevious, vCurrent, vRotated;

	for (int i = 0; i <= pPolygon->m_nVertices; i++)
	{
		vertex = pPolygon->m_pVertices[i % pPolygon->m_nVertices];
		vRotated = vCurrent = vertex.m_vPosition;
		//World/View/Projection Transformation
		D3DXVec3TransformCoord(&vCurrent, &vCurrent, &mtxTransform);
		//Perspective Divide
		if (vCurrent.z != 0.0f)
		{
			vCurrent.x = vCurrent.x / vCurrent.z;
			vCurrent.y = vCurrent.y / vCurrent.z;
		}
		if ((vCurrent.z < 0.0f) || (vCurrent.z > 1.0f)) continue;
		//Screen Transformation
		vCurrent.x = +vCurrent.x * (m_pCamera->m_cxViewport * 0.5f)
			+ m_pCamera->m_xViewport + (m_pCamera->m_cxViewport * 0.5f);

		vCurrent.y = -vCurrent.y * (m_pCamera->m_cyViewport * 0.5f)
			+ m_pCamera->m_yViewport + (m_pCamera->m_cyViewport * 0.5f);

		if ((i != 0) && (vCurrent.z > 0.0f))
		{
			HPEN hPen = ::CreatePen(PS_SOLID, 0, vertex.m_dwColor);
			HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
			::MoveToEx(m_hDCFrameBuffer, (long)vPrevious.x, (long)vPrevious.y, NULL);
			::LineTo(m_hDCFrameBuffer, (long)vCurrent.x, (long)vCurrent.y);
			::SelectObject(m_hDCFrameBuffer, hOldPen);
			::DeleteObject(hPen);
		}
		vPrevious = vCurrent;
	}
}

void CGameFramework::DrawObject(CGameObject *pObject, D3DXMATRIX& mtxViewProject)
{
	D3DXMATRIX mtxTransform = pObject->m_mtxWorld * mtxViewProject;

	CMesh *pMesh = pObject->m_pMesh;

	for (int j = 0; j < pMesh->m_nPolygons; j++)
		DrawPrimitive(pMesh->m_ppPolygons[j], mtxTransform);
}

void CGameFramework::FrameAdvance()
{
	if (!m_bActive) return;

	ProcessInput();

	AnimateObjects();

	ClearFrameBuffer(0x00FFFFFF);

	D3DXMATRIX mtxViewProject = m_pCamera->m_mtxView * m_pCamera->m_mtxProjection;


	DrawObject(m_pOutWall, mtxViewProject);
	DrawObject(m_pTank, mtxViewProject);

	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_pObjects[i].m_flag == 0)
			DrawObject(&m_pObjects[i], mtxViewProject);
	}


	if (m_bBulletCheck)
		DrawObject(m_pBullet, mtxViewProject);

	PresentFrameBuffer();
}



void CGameFramework::CollisionOutWall(CGameObject& cube, const CGameObject& wall)
{
	D3DXPLANE plane[6];
	GetPlaneFromPoint(plane, wall);

	D3DXVECTOR3 vec3tmp;
	D3DXVECTOR4 vec4tmp;

	for (int i = 0; i < 6; ++i)
	{
		GetVectorFromVertex(&vec3tmp, cube.m_pMesh->m_ppPolygons[i]->m_pVertices[0]);

		D3DXVec3Transform(&vec4tmp, &vec3tmp, &cube.m_mtxWorld);

		if (D3DXPlaneDot(&plane[i], &vec4tmp) >= 0)
		{
			D3DXVECTOR3 r;

			GetReflexVector(&r, cube.m_vMovingDirection, plane[i]);

			cube.SetMovingDirection(r);
		}
	}
}

bool CGameFramework::CrashDectection(CGameObject& cube, const CGameObject& wall)
{
	D3DXPLANE plane[6];
	GetPlaneFromPoint(plane, wall);

	D3DXVECTOR3 vec3tmp;
	D3DXVECTOR4 vec4tmp;

	for (int i = 0; i < 6; ++i)
	{
		GetVectorFromVertex(&vec3tmp, cube.m_pMesh->m_ppPolygons[i]->m_pVertices[0]);

		D3DXVec3Transform(&vec4tmp, &vec3tmp, &cube.m_mtxWorld);

		if (D3DXPlaneDot(&plane[i], &vec4tmp) >= 0)
		{
			return true;
		}
	}
	return false;
}

bool CGameFramework::CollisionInnerWall(CGameObject& cube, CGameObject& wall)
{
	float fDistance;
	D3DXVECTOR3 vDiff;


	vDiff.x = wall.GetPosition().x - cube.GetPosition().x;
	vDiff.y = wall.GetPosition().y - cube.GetPosition().y;
	vDiff.z = wall.GetPosition().z - cube.GetPosition().z;

	fDistance = D3DXVec3Length(&vDiff);

	if (fDistance <= (2))
		return true;
	return false;
}

void CGameFramework::GetPlaneFromPoint(D3DXPLANE* out, const CGameObject& obj)
{
	D3DXVECTOR3 point[3];

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			point[j].x = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].m_vPosition.x;
			point[j].y = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].m_vPosition.y;
			point[j].z = obj.m_pMesh->m_ppPolygons[i]->m_pVertices[j].m_vPosition.z;
		}
		D3DXPlaneFromPoints(&out[i], &point[0], &point[1], &point[2]);
	}
}

void CGameFramework::GetVectorFromVertex(D3DXVECTOR3* out, const CVertex& vertex)
{
	out->x = vertex.m_vPosition.x;
	out->y = vertex.m_vPosition.y;
	out->z = vertex.m_vPosition.z;
}

void CGameFramework::GetReflexVector(D3DXVECTOR3* out, const D3DXVECTOR3& dir, const D3DXPLANE& plane)
{
	D3DXVECTOR3 n(plane.a, plane.b, plane.c);

	*out = dir - 2 * D3DXVec3Dot(&dir, &n) * n;
}
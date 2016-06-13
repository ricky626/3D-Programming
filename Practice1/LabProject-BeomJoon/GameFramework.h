#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"

#include "GameObject.h"

#define CLIENT_WIDTH	640
#define CLIENT_HEIGHT	480

class CGameFramework
{
public:
	CGameFramework(void);
	~CGameFramework(void);

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	void FrameAdvance();
	void ResetDisplay(int nClientWidth = 0, int nClientHeight = 0);

	void SetActive(bool bActive) { m_bActive = bActive; }

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;
	HMENU						m_hMenu;

	bool						m_bActive;

	HDC							m_hDCFrameBuffer;
	HBITMAP						m_hBitmapFrameBuffer;
	HBITMAP						m_hBitmapSelect;

	int							m_xViewport;
	int							m_yViewport;
	int							m_cxViewport;
	int							m_cyViewport;

	CCamera						*m_pCamera;

	int							m_nObjects;
	CGameObject					*m_pObjects;

	CGameObject					*m_pOutWall;
	CGameObject					*m_pTank;
	CGameObject					*m_pBullet;

	bool						m_bBulletCheck;

public:
	void BuildFrameBuffer();
	void ClearFrameBuffer(DWORD dwColor);
	void PresentFrameBuffer();

	void BuildObjects();
	void ReleaseObjects();
	void SetupGameState();
	void SetupRenderStates();
	void AnimateObjects();
	void ProcessInput();

	void DrawObject(CGameObject *pObject, D3DXMATRIX& mtxViewProject);
	void DrawPrimitive(CPolygon *pPolygon, D3DXMATRIX& mtxTransform);

	CMesh *BuildCubeMesh(float size, UINT rcolor, UINT gcolor, UINT bcolor);


	POINT						m_ptOldCursorPos;

	void CollisionOutWall(CGameObject& cube, const CGameObject& wall);
	bool CrashDectection(CGameObject& cube, const CGameObject& wall);

	bool CollisionInnerWall(CGameObject& cube, CGameObject& wall);

	void GetPlaneFromPoint(D3DXPLANE* out, const CGameObject& obj);
	void GetVectorFromVertex(D3DXVECTOR3* out, const CVertex& vertex);
	void GetReflexVector(D3DXVECTOR3* out, const D3DXVECTOR3& dir, const D3DXPLANE& wall);
};

#endif 


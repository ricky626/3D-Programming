#pragma once
#include "Object.h"

class CScene
{
private:
	CGameObject **m_ppObjects;
	int m_nObjects;

	CGameObject					*m_pOutWall;






public:
	CScene();
	~CScene();

	float planeX = 40;
	float planeY = 40;
	float planeZ = 10;
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	void ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera);

	

};


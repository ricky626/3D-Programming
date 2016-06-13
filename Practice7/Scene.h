#pragma once

#include "Shader.h"

class CScene
{
private:
	//���� ���̴����� ����Ʈ(�迭)�̴�.
	CShader *m_pShaders;
	int m_nShaders;


public:
	CScene();
	~CScene();	
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
};


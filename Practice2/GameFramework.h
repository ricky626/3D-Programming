#pragma once
#include "Timer.h"
#include "Player.h"
#include "Scene.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	//����̽� �������̽��� ���� ������. ���ҽ� ������ ���Ͽ� �ʿ�
	ID3D11Device *m_pd3dDevice;
	//���� ü�� �������̽��� ���� ������. ���÷��̸� �����ϱ� ���Ͽ� �ʿ�
	IDXGISwapChain *m_pDXGISwapChain;
	//����̽� ���ؽ�Ʈ�� ���� ������. ���������� ������ ���Ͽ� �ʿ�
	ID3D11DeviceContext *m_pd3dDeviceContext;
	//���� Ÿ�� �� �������̽��� ���� ������.
	ID3D11RenderTargetView *m_pd3dRenderTargetView;

	//������ ���� �����ӿ�ũ���� ����� Ÿ�̸��̴�.
	CGameTimer m_GameTimer;

	//������ ������ ���(Scene)�� �����ϴ� ��ü�� ���� �����͸� ��Ÿ����.
	CScene *m_pScene;

	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
	_TCHAR m_pszBuffer[50];

	CPlayer *m_pPlayer;

public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	//����̽� ���� ü�� ����̽� ���ؽ�Ʈ ����̽��� ���õ� �並 �����ϴ� �Լ�.
	bool CreateRenderTargetView();
	bool CreateDirect3DDisplay();

	//�������� �޽�, ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�. 
	void BuildObjects();
	void ReleaseObjects();

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};


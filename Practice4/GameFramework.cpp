#include "stdafx.h"
#include "GameFramework.h"

bool isNight = true;

CGameFramework::CGameFramework()
{
	m_nPlayers = 0;
	m_ppPlayers = NULL;

	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
	_tcscpy_s(m_pszBuffer, _T("LapProject ("));

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;


}

CGameFramework::~CGameFramework()
{
}

//���� �Լ��� ���� ���α׷��� ����Ǹ� ȣ��ȴٴ� �Ϳ� �����϶�. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. 
	if (!CreateDirect3DDisplay()) return(false);

	//�������� ��ü(���� ���� ��ü)�� �����Ѵ�. 
	BuildObjects();

	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return(true);
}


bool CGameFramework::CreateRenderTargetView()
{
	HRESULT hResult = S_OK;

	//���� ü���� �ĸ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//���� Ÿ�� �並 �����ϰ� ���-���� �ܰ迡 �����Ѵ�.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

	return(true);
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//����̽��� �����ϱ� ���Ͽ� �õ��� Ư�� ������ ������ ��Ÿ����.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//������ ���� ü���� �����ϴ� ����ü�̴�.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�.
	// ������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//���� Ÿ�� �並 �����ϴ� �Լ��� ȣ���Ѵ�.
	if (!CreateRenderTargetDepthStencilView()) return(false);

	//SetViewport();

	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//���콺 ĸ�ĸ� �ϰ� ���� ���콺 ��ġ�� �����´�.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//���콺 ĸ�ĸ� �����Ѵ�.
		ReleaseCapture();
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
		case VK_F2:
		case VK_F3:
			m_ppPlayers[0]->ChangeCamera(m_pd3dDevice, (wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
			break;
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_SPACE :
			if (m_pScene->m_pLights->m_pLights[1].m_bEnable)
				m_pScene->m_pLights->m_pLights[1].m_bEnable = false;
			else
				m_pScene->m_pLights->m_pLights[1].m_bEnable = true;
			break;
		case VK_RETURN :
			if (m_pScene->m_pLights->m_pLights[3].m_bEnable){
				m_pScene->m_pLights->m_pLights[3].m_bEnable = false;
				isNight = false;
			}
			else{
				m_pScene->m_pLights->m_pLights[3].m_bEnable = true;
				isNight = true;
			}
			break;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
	{
					m_nWndClientWidth = LOWORD(lParam);
					m_nWndClientHeight = HIWORD(lParam);

					m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

					if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
					if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
					if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

					m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

					CreateRenderTargetDepthStencilView();

					if (m_ppPlayers)
					{
						CCamera *pCamera = m_ppPlayers[0]->GetCamera();
						pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
					}
					break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

//���� �Լ��� ���� ���α׷��� ����� �� ȣ��ȴٴ� �Ϳ� �����϶�. 
void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}


void CGameFramework::BuildObjects()
{
	m_pScene = new CScene();

	m_nPlayers = 1;
	m_ppPlayers = new CPlayer*[m_nPlayers];

	CTerrainPlayer *pTerrainPlyer = new CTerrainPlayer(m_pd3dDevice);
	pTerrainPlyer->ChangeCamera(m_pd3dDevice, THIRD_PERSON_CAMERA, m_GameTimer.GetTimeElapsed());

	CCamera *pCamera = pTerrainPlyer->GetCamera();
	pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 90.0f);
	pCamera->GenerateViewMatrix();

	if (m_pScene){
		m_pScene->BuildObjects(m_pd3dDevice);
		m_pScene->BuildLights(m_pd3dDevice);
	}
	
	CHeightMapTerrain *pTerrain = m_pScene->m_ppShaders[1]->m_pTerrain;
	D3DXVECTOR3 vScale = pTerrain->m_pHeightMap->GetScale();
	float cxTerrain = (float)pTerrain->m_pHeightMap->GetHeightMapWidth();
	float czTerrain = (float)pTerrain->m_pHeightMap->GetHeightMapLength();
	/*������ xz-����� ����� �÷��̾ ��ġ�ϵ��� �Ѵ�. �÷��̾��� y-��ǥ�� ������ ���� ���� ũ�� �߷��� �ۿ��ϵ��� �÷��̾ �����Ͽ����Ƿ� �÷��̾�� ���������� �ϰ��ϰ� �ȴ�.*/
	pTerrainPlyer->SetPosition(D3DXVECTOR3(cxTerrain*0.5f*vScale.x, 1400.0f, czTerrain*0.5f*vScale.z));
	//�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
	pTerrainPlyer->SetPlayerUpdatedContext(m_pScene->m_ppShaders[1]->m_pTerrain);
	//ī�޶��� ��ġ�� ����� �� ������ ������ ���� ī�޶��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
	pTerrainPlyer->SetCameraUpdatedContext(m_pScene->m_ppShaders[1]->m_pTerrain);
	
	m_ppPlayers[0] = pTerrainPlyer;
}


void CGameFramework::ReleaseObjects()
{
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	if (m_ppPlayers)
	{
		for (int j = 0; j < m_nPlayers; j++) delete m_ppPlayers[j];
		delete[] m_ppPlayers;
	}
}


void CGameFramework::ProcessInput()
{
	bool bProcessedByScene = false;
	if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDirection = 0;

		if (GetKeyboardState(pKeyBuffer))
		{
			if (pKeyBuffer[0x57] & 0xF0) // w
				dwDirection |= DIR_FORWARD;

			if (pKeyBuffer[0x53] & 0xF0)
				dwDirection |= DIR_BACKWARD; // s

			if (pKeyBuffer[0x41] & 0xF0)	// a
				dwDirection |= DIR_LEFT;

			if (pKeyBuffer[0x44] & 0xF0)	// d
				dwDirection |= DIR_RIGHT;

			//if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
			//if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
		}
		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;

		if (GetCapture() == m_hWnd)
		{
			//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
			SetCursor(NULL);
			//���� ���콺 Ŀ���� ��ġ�� �����´�.
			GetCursorPos(&ptCursorPos);
			//���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
			//cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			//cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;

			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f; // ���� �� �˻�

			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		//�÷��̾ �̵��ϰų�(dwDirection) ȸ���Ѵ�(cxDelta �Ǵ� cyDelta).
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				/*cxDelta�� y-���� ȸ���� ��Ÿ���� cyDelta�� x-���� ȸ���� ��Ÿ����. ������ ���콺 ��ư�� ������ ��� cxDelta�� z-���� ȸ���� ��Ÿ����.*/
				if (pKeyBuffer[VK_LBUTTON] & 0xF0)
					m_ppPlayers[0]->Rotate(0.0f, cxDelta, 0.0f); // y ������ cyDelta ��ŭ ȸ��
				//else
				//	m_ppPlayers[0]->Rotate(cyDelta, cxDelta, 0.0f);
			}
			// �÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�).
			// �̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�. �÷��̾��� �̵� �ӷ��� 500/�ʷ� �����Ѵ�.
			// ���� �÷��̾��� �̵� �ӷ��� �ִٸ� �� ���� ����Ѵ�. // 100���� �ٲ�
			if (dwDirection) m_ppPlayers[0]->Move(dwDirection, 300.0f * m_GameTimer.GetTimeElapsed(), true);
		}
	}
	//�÷��̾ ������ �̵��ϰ� ī�޶� �����Ѵ�. �߷°� �������� ������ �ӵ� ���Ϳ� �����Ѵ�.
	m_ppPlayers[0]->Update(m_GameTimer.GetTimeElapsed());
}


void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick();

	ProcessInput();

	AnimateObjects();
	float fClearColor[4];
	if (!isNight){
		 fClearColor[0] = 0.0f;  fClearColor[1] = 0.125f;
		 fClearColor[2] = 0.3f;	 fClearColor[3] = 1.0f;
	}
	else{
		fClearColor[0] = 178.0/255.0;  fClearColor[1] = 235.0/255.0;
		fClearColor[2] = 244.0/255.0;	 fClearColor[3] = 1.0f;
	}
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	CCamera *pCamera = NULL;
	for (int i = 0; i < m_nPlayers; i++)
	{
		if (m_ppPlayers[i]) m_ppPlayers[i]->UpdateShaderVariables(m_pd3dDeviceContext);
		pCamera = m_ppPlayers[i]->GetCamera();
		if (m_pScene)
		{
			m_pScene->Render(m_pd3dDeviceContext, pCamera);
		}
		//3��Ī ī�޶��� �� �÷��̾ �������Ѵ�.
		for (int j = 0; j < m_nPlayers; j++) if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);
	}

	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}

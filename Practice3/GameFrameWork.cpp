#include "GameFrameWork.h"

CGameFrameWork::CGameFrameWork()
{
	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
	_tcscpy_s(m_pszBuffer, _T("LapProject ("));

	m_nPlayers = 0;
	m_ppPlayers = NULL;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;
}


CGameFrameWork::~CGameFrameWork()
{
}

//���� �Լ��� ���� ���α׷��� ����Ǹ� ȣ��ȴٴ� �Ϳ� �����϶�. 
bool CGameFrameWork::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. 
	if (!CreateDirect3DDisplay()) return(false);

	//�������� ��ü(���� ���� ��ü)�� �����Ѵ�. 
	BuildObjects();

	return(true);
}

bool CGameFrameWork::CreateRenderTargetDepthStencilView()
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


bool CGameFrameWork::CreateDirect3DDisplay()
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
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�. ������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//���� Ÿ�� �並 �����ϴ� �Լ��� ȣ���Ѵ�.
	if (!CreateRenderTargetDepthStencilView()) return(false);

	return(true);
}

void CGameFrameWork::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFrameWork::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static int rad = 0;
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F1: case VK_F2: case VK_F3:
			m_ppPlayers[0]->ChangeCamera(m_pd3dDevice, (wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
			break;
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case 'S' :
			m_pScene->m_pLights->m_pLights[1].m_bEnable = false;
			break;
		case 'A' :
			m_pScene->m_pLights->m_pLights[1].m_bEnable = true;
			break;
		case VK_SPACE :
			rad += 1;
			m_ppPlayers[0]->Rotate(0.0f, rad, 0.0f);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFrameWork::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE:
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(2, m_nWndClientWidth, m_nWndClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();
		if (m_ppPlayers)
		{
			CCamera *pCamera = m_ppPlayers[0]->GetCamera();
			pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
		}
		break;
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
void CGameFrameWork::OnDestroy()
{
	//���� ��ü�� �Ҹ��Ѵ�. 
	ReleaseObjects();

	//Direct3D�� ���õ� ��ü�� �Ҹ��Ѵ�. 
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFrameWork::BuildObjects()
{
	m_pScene = new CScene();

	// �÷��̾� ��ü�� �����Ѵ�.
	m_nPlayers = 1;
	m_ppPlayers = new CPlayer*[m_nPlayers];
	m_ppPlayers[0] = new CPlayer();

	CAirplanePlayer *pAirplanePlyer = new CAirplanePlayer(m_pd3dDevice);

	//�÷��̾��� ī�޶� �����̽�-�� ī�޶�� �����Ѵ�.
	pAirplanePlyer->ChangeCamera(m_pd3dDevice, SPACESHIP_CAMERA, m_GameTimer.GetTimeElapsed());

	//ī�޶� ��ü�� �����ϰ� ��-��Ʈ�� �����Ѵ�. 
	CCamera *pCamera = pAirplanePlyer->GetCamera();
	pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
	pCamera->GenerateViewMatrix();

	//�÷��̾��� ��ġ�� �����̽�-�� ī�޶�� �����Ѵ�.
	pAirplanePlyer->SetPosition(D3DXVECTOR3(-1600.0f, -1100 + 200, -800.0f));

	m_ppPlayers[0] = pAirplanePlyer;

	if (m_pScene){
		m_pScene->BuildObjects(m_pd3dDevice);
		m_pScene->BuildLights(m_pd3dDevice);
	}
}

void CGameFrameWork::ReleaseObjects()
{
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	if (m_ppPlayers)
	{
		for (int j = 0; j < m_nPlayers; ++j) delete m_ppPlayers[j];
		delete[] m_ppPlayers;
	}
	if (m_pd3dcbColor) m_pd3dcbColor->Release();
}

void CGameFrameWork::ProcessInput()
{
	bool bProcessedByScene = false;
	if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDirection = 0;
		/*Ű������ ���� ������ ��ȯ�Ѵ�. ȭ��ǥ Ű(���桯, ���硯, ���衯, ���顯)�� ������ 
		�÷��̾ ������/����(���� x-��), ��/��(���� z-��)�� �̵��Ѵ�. 
		��Page Up���� ��Page Down�� Ű�� ������ �÷��̾ ��/�Ʒ�(���� y-��)�� �̵��Ѵ�.*/
		if (GetKeyboardState(pKeyBuffer))
		{
			if (pKeyBuffer[VK_UP] & 0xF0){
				dwDirection |= DIR_FORWARD;
				m_ppPlayers[0]->dir = DIR_FORWARD;
			}
			if (pKeyBuffer[VK_DOWN] & 0xF0){
				dwDirection |= DIR_BACKWARD;
				m_ppPlayers[0]->dir = DIR_BACKWARD;
			}
			if (pKeyBuffer[VK_LEFT] & 0xF0){
				dwDirection |= DIR_LEFT;
				m_ppPlayers[0]->dir = DIR_LEFT;
			}
			if (pKeyBuffer[VK_RIGHT] & 0xF0){
				dwDirection |= DIR_RIGHT;
				m_ppPlayers[0]->dir = DIR_RIGHT;
			}
			if (pKeyBuffer[VK_PRIOR] & 0xF0){
				dwDirection |= DIR_UP;
				m_ppPlayers[0]->dir = DIR_UP;
			}
			if (pKeyBuffer[VK_NEXT] & 0xF0){
				dwDirection |= DIR_DOWN;
				m_ppPlayers[0]->dir = DIR_DOWN;
			}
		}
		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		/*���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�. 
		���콺 ���� �Ǵ� ������ ��ư�� ������ ���� 
		�޽���(WM_LBUTTONDOWN, WM_RBUTTONDOWN)�� ó���� �� ���콺�� ĸ���Ͽ���. 
		�׷��Ƿ� ���콺�� ĸ�ĵ� ���� ���콺 ��ư�� ������ ���¸� �ǹ��Ѵ�. 
		���콺�� �¿� �Ǵ� ���Ϸ� �����̸� �÷��̾ x-�� �Ǵ� y-������ ȸ���Ѵ�.*/
		if (GetCapture() == m_hWnd)
		{
			//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
			SetCursor(NULL);
			//���� ���콺 Ŀ���� ��ġ�� �����´�.
			GetCursorPos(&ptCursorPos);
			//���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		//�÷��̾ �̵��ϰų�(dwDirection) ȸ���Ѵ�(cxDelta �Ǵ� cyDelta).
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				/*cxDelta�� y-���� ȸ���� ��Ÿ���� cyDelta�� x-���� ȸ���� ��Ÿ����. 
				������ ���콺 ��ư�� ������ ��� cxDelta�� z-���� ȸ���� ��Ÿ����.*/
				if (pKeyBuffer[VK_RBUTTON] & 0xF0)
					m_ppPlayers[0]->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_ppPlayers[0]->Rotate(cyDelta, cxDelta, 0.0f);
			}
			/*�÷��̾ dwDirection �������� �̵��Ѵ�(�����δ� �ӵ� ���͸� �����Ѵ�). 
			�̵� �Ÿ��� �ð��� ����ϵ��� �Ѵ�. �÷��̾��� �̵� �ӷ��� 500/�ʷ� �����Ѵ�. 
			���� �÷��̾��� �̵� �ӷ��� �ִٸ� �� ���� ����Ѵ�.*/
			if (dwDirection) m_ppPlayers[0]->Move(dwDirection, 500.0f * m_GameTimer.GetTimeElapsed(), true);
		}
	}
	//�÷��̾ ������ �̵��ϰ� ī�޶� �����Ѵ�. �߷°� �������� ������ �ӵ� ���Ϳ� �����Ѵ�.
	m_ppPlayers[0]->Update(m_GameTimer.GetTimeElapsed());
	m_ppPlayers[0]->SetRect();
}

void CGameFrameWork::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFrameWork::AI()
{
	CShader** pShaders = m_pScene->m_ppShaders;

	for (int j = 0; j <= TANK_OBJECT9; ++j)
	{
		if (j % 2){
			pShaders[0]->m_ppObjects[j]->MoveStrafe(-1);
		}
		else{
			pShaders[0]->m_ppObjects[j]->MoveForward(-1);		
		}

		pShaders[0]->m_ppObjects[j]->Rotate(0, j*0.05, 0);
		pShaders[0]->m_ppObjects[j]->SetRect();
		/* ��ֹ��̶� �浹üũ */
		for (int i = WALL_OBJECT0; i < OBJECT_END; ++i)
		{
			if (i >= MAP_OBJECT_X0 && i <= MAP_OBJECT_Z1) continue;
			RECT tank_rt = pShaders[0]->m_ppObjects[j]->m_rt;
			RECT object_rt = pShaders[0]->m_ppObjects[i]->m_rt;
			RECT player_rt = m_ppPlayers[0]->m_rt;
			// ��ũ�� ��ֹ� OK
			if (tank_rt.left < object_rt.right && tank_rt.right > object_rt.left &&
				tank_rt.top < object_rt.bottom && tank_rt.bottom > object_rt.top)
			{
				pShaders[0]->m_ppObjects[j]->MoveForward(1);
				pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
				pShaders[0]->m_ppObjects[j]->SetRect();
			}
			// �÷��̾�� ��ֹ� OK
			if (object_rt.left < player_rt.right && object_rt.right > player_rt.left &&
				object_rt.top < player_rt.bottom && object_rt.bottom > player_rt.top)
			{
				float new_x;
				float new_y;
				float new_z;
				D3DXVECTOR3 new_v;
				if (m_ppPlayers[0]->dir == DIR_FORWARD)
				{
					new_x = m_ppPlayers[0]->GetPosition().x;
					new_y = m_ppPlayers[0]->GetPosition().y;
					new_z = m_ppPlayers[0]->GetPosition().z - 10;
					new_v.x = new_x; new_v.y = new_y; new_v.z = new_z;
					m_ppPlayers[0]->SetPosition(new_v);
				}
				if (m_ppPlayers[0]->dir == DIR_BACKWARD)
				{
					new_x = m_ppPlayers[0]->GetPosition().x;
					new_y = m_ppPlayers[0]->GetPosition().y;
					new_z = m_ppPlayers[0]->GetPosition().z + 10;
					new_v.x = new_x; new_v.y = new_y; new_v.z = new_z;
					m_ppPlayers[0]->SetPosition(new_v);
				}
				if (m_ppPlayers[0]->dir == DIR_LEFT)
				{
					new_x = m_ppPlayers[0]->GetPosition().x + 10;
					new_y = m_ppPlayers[0]->GetPosition().y;
					new_z = m_ppPlayers[0]->GetPosition().z;
					new_v.x = new_x; new_v.y = new_y; new_v.z = new_z;
					m_ppPlayers[0]->SetPosition(new_v);
				}
				if (m_ppPlayers[0]->dir == DIR_RIGHT)
				{
					new_x = m_ppPlayers[0]->GetPosition().x - 10;
					new_y = m_ppPlayers[0]->GetPosition().y;
					new_z = m_ppPlayers[0]->GetPosition().z;
					new_v.x = new_x; new_v.y = new_y; new_v.z = new_z;
					m_ppPlayers[0]->SetPosition(new_v);
				}
				pShaders[0]->m_ppObjects[j]->SetRect();
			}
		}
		/* ��ũ�� �� */ // OK
		RECT tank_rt = pShaders[0]->m_ppObjects[j]->m_rt;
		if (tank_rt.left < -2000 + pShaders[0]->m_ppObjects[j]->m_nWidth)
		{
			pShaders[0]->m_ppObjects[j]->MoveStrafe(-10);
			pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
		}
		if (tank_rt.right > 2000)
		{
			pShaders[0]->m_ppObjects[j]->MoveStrafe(-10);
			pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
		}
		if (tank_rt.top > 850)
		{
			pShaders[0]->m_ppObjects[j]->MoveForward(10);
			pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
		}
		if (tank_rt.bottom < -850)
		{
			pShaders[0]->m_ppObjects[j]->MoveForward(10);
			pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
		}
		pShaders[0]->m_ppObjects[j]->SetRect();

		/* �÷��̾� ��ũ*/ // OK
		RECT player_rt = m_ppPlayers[0]->m_rt;
		if (tank_rt.left < player_rt.right && tank_rt.right > player_rt.left &&
			tank_rt.top < player_rt.bottom && tank_rt.bottom > player_rt.top)
		{
			//pShaders[0]->m_ppObjects[j]->SetActive(false);
		}
		//m_pScene->m_pLights[j + 12].m_d3dxvCameraPosition.x = pShaders[0]->m_ppObjects[j]->GetPosition().x;
		//m_pScene->m_pLights[j + 12].m_d3dxvCameraPosition.y = pShaders[0]->m_ppObjects[j]->GetPosition().y;
		//m_pScene->m_pLights[j + 12].m_d3dxvCameraPosition.z = pShaders[0]->m_ppObjects[j]->GetPosition().z;
	}

	if (m_ppPlayers[0]->m_rt.left < -2000 )
	{
		m_ppPlayers[0]->MoveStrafe(-100);
	}
	if (m_ppPlayers[0]->m_rt.right > 2000)
	{
		m_ppPlayers[0]->MoveStrafe(-100);
	}
	if (m_ppPlayers[0]->m_rt.top > 850)
	{
		m_ppPlayers[0]->MoveForward(100);
	}
	if (m_ppPlayers[0]->m_rt.bottom < -850)
	{
		m_ppPlayers[0]->MoveForward(100);
	}
}

void CGameFrameWork::FrameAdvance()
{
	m_GameTimer.Tick();

	ProcessInput();

	// �浹üũ
	AI();

	AnimateObjects();

	float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, 
		D3D11_CLEAR_DEPTH, 1.0f, 0);
	CCamera *pCamera = NULL;
	for (int i = 0; i < m_nPlayers; i++)
	{
		if (m_ppPlayers[i])
		{
			m_ppPlayers[i]->UpdateShaderVariables(m_pd3dDeviceContext);
			pCamera = m_ppPlayers[i]->GetCamera();
		}
		if (m_pScene) m_pScene->Render(m_pd3dDeviceContext, pCamera);
		//3��Ī ī�޶��� �� �÷��̾ �������Ѵ�.
		for (int j = 0; j < m_nPlayers; ++j) if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);
	}
	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}

// �� ���� ���� �˱�~
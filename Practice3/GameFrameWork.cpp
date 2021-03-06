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

//다음 함수는 응용 프로그램이 실행되면 호출된다는 것에 유의하라. 
bool CGameFrameWork::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인 등을 생성하는 함수를 호출한다. 
	if (!CreateDirect3DDisplay()) return(false);

	//렌더링할 객체(게임 월드 객체)를 생성한다. 
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
	
	//디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서를 나타낸다.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//생성할 스왑 체인을 서술하는 구조체이다.
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
	//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성한다. 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성한다.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//렌더 타겟 뷰를 생성하는 함수를 호출한다.
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
		/*윈도우의 크기가 변경될 때(현재는 “Alt+Enter“ 전체 화면 모드와 윈도우 모드로 전환될 때) 스왑 체인의 후면버퍼 크기를 조정하고 후면버퍼에 대한 렌더 타겟 뷰를 다시 생성한다. */
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
		//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//마우스 캡쳐를 해제한다.
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

//다음 함수는 응용 프로그램이 종료될 때 호출된다는 것에 유의하라. 
void CGameFrameWork::OnDestroy()
{
	//게임 객체를 소멸한다. 
	ReleaseObjects();

	//Direct3D와 관련된 객체를 소멸한다. 
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

	// 플레이어 객체를 생성한다.
	m_nPlayers = 1;
	m_ppPlayers = new CPlayer*[m_nPlayers];
	m_ppPlayers[0] = new CPlayer();

	CAirplanePlayer *pAirplanePlyer = new CAirplanePlayer(m_pd3dDevice);

	//플레이어의 카메라를 스페이스-쉽 카메라로 변경한다.
	pAirplanePlyer->ChangeCamera(m_pd3dDevice, SPACESHIP_CAMERA, m_GameTimer.GetTimeElapsed());

	//카메라 객체를 생성하고 뷰-포트를 설정한다. 
	CCamera *pCamera = pAirplanePlyer->GetCamera();
	pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
	pCamera->GenerateViewMatrix();

	//플레이어의 위치를 스페이스-쉽 카메라로 변경한다.
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
		/*키보드의 상태 정보를 반환한다. 화살표 키(‘→’, ‘←’, ‘↑’, ‘↓’)를 누르면 
		플레이어를 오른쪽/왼쪽(로컬 x-축), 앞/뒤(로컬 z-축)로 이동한다. 
		‘Page Up’과 ‘Page Down’ 키를 누르면 플레이어를 위/아래(로컬 y-축)로 이동한다.*/
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
		/*마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다. 
		마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의 
		메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 마우스를 캡쳐하였다. 
		그러므로 마우스가 캡쳐된 것은 마우스 버튼이 눌려진 상태를 의미한다. 
		마우스를 좌우 또는 상하로 움직이면 플레이어를 x-축 또는 y-축으로 회전한다.*/
		if (GetCapture() == m_hWnd)
		{
			//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
			SetCursor(NULL);
			//현재 마우스 커서의 위치를 가져온다.
			GetCursorPos(&ptCursorPos);
			//마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		//플레이어를 이동하거나(dwDirection) 회전한다(cxDelta 또는 cyDelta).
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				/*cxDelta는 y-축의 회전을 나타내고 cyDelta는 x-축의 회전을 나타낸다. 
				오른쪽 마우스 버튼이 눌려진 경우 cxDelta는 z-축의 회전을 나타낸다.*/
				if (pKeyBuffer[VK_RBUTTON] & 0xF0)
					m_ppPlayers[0]->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_ppPlayers[0]->Rotate(cyDelta, cxDelta, 0.0f);
			}
			/*플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다). 
			이동 거리는 시간에 비례하도록 한다. 플레이어의 이동 속력은 500/초로 가정한다. 
			만약 플레이어의 이동 속력이 있다면 그 값을 사용한다.*/
			if (dwDirection) m_ppPlayers[0]->Move(dwDirection, 500.0f * m_GameTimer.GetTimeElapsed(), true);
		}
	}
	//플레이어를 실제로 이동하고 카메라를 갱신한다. 중력과 마찰력의 영향을 속도 벡터에 적용한다.
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
		/* 장애물이랑 충돌체크 */
		for (int i = WALL_OBJECT0; i < OBJECT_END; ++i)
		{
			if (i >= MAP_OBJECT_X0 && i <= MAP_OBJECT_Z1) continue;
			RECT tank_rt = pShaders[0]->m_ppObjects[j]->m_rt;
			RECT object_rt = pShaders[0]->m_ppObjects[i]->m_rt;
			RECT player_rt = m_ppPlayers[0]->m_rt;
			// 탱크와 장애물 OK
			if (tank_rt.left < object_rt.right && tank_rt.right > object_rt.left &&
				tank_rt.top < object_rt.bottom && tank_rt.bottom > object_rt.top)
			{
				pShaders[0]->m_ppObjects[j]->MoveForward(1);
				pShaders[0]->m_ppObjects[j]->Rotate(0, rand() % 90, 0);
				pShaders[0]->m_ppObjects[j]->SetRect();
			}
			// 플레이어랑 장애물 OK
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
		/* 탱크랑 맵 */ // OK
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

		/* 플레이어 탱크*/ // OK
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

	// 충돌체크
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
		//3인칭 카메라일 때 플레이어를 렌더링한다.
		for (int j = 0; j < m_nPlayers; ++j) if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);
	}
	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}

// ※ 파일 포맷 알기~
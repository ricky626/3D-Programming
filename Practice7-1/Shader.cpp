#include "stdafx.h"
#include "Shader.h"

//���� ��ȯ ����� ���� ��� ���۴� ���̴� ��ü�� ����(static) ������ ����̴�.
ID3D11Buffer *CShader::m_pd3dcbWorldMatrix = NULL;

CShader::CShader()
{
	m_ppObjects = NULL;
	m_nObjects = 0;

	m_pd3dVertexShader = NULL;
	m_pd3dVertexLayout = NULL;
	m_pd3dPixelShader = NULL;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
}


void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
	/*IA �ܰ迡 ������ �Է�-���̾ƿ��� �����Ѵ�. ���� ������ �� ���Ұ� CVertex Ŭ������ ��� ����(D3DXVECTOR3 ��, �Ǽ� �� ��)�̹Ƿ� �Է�-���̾ƿ��� �Ǽ�(32-��Ʈ) 3���� �����Ǹ� �ø�ƽ�� ��POSITION���̰� ���� ���������� ǥ���Ѵ�.*/
	//�ϳ��� ������ ��ġ ����(�Ǽ� 3��)�� ����(�Ǽ� 4��)�� �����Ѵ�. 
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	/*
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	*/
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	//���� ��Effect.fx������ ����-���̴��� ���� �Լ��� "VS"�� ����-���̴��� �����Ѵ�. 
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VS", "vs_5_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	//���� ��Effect.fx������ �ȼ�-���̴��� ���� �Լ��� "PS"�� �ȼ�-���̴��� �����Ѵ�. 
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PS", "ps_5_0", &m_pd3dPixelShader);

	CreateShaderVariables(pd3dDevice);
}

void CShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	//���� ��ȯ ����� ���� ��� ���۸� �����Ѵ�.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbWorldMatrix);
}

void CShader::ReleaseShaderVariables()
{
	//���� ��ȯ ����� ���� ��� ���۸� ��ȯ�Ѵ�.
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
}

void CShader::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	//���� ��ȯ ����� ��� ���ۿ� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);

	//��� ���۸� ����̽��� ����(VS_SLOT_WORLD_MATRIX)�� �����Ѵ�.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CShader::BuildObjects(ID3D11Device *pd3dDevice)
{
}

void CShader::ReleaseObjects()
{
	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) if (m_ppObjects[j]) delete m_ppObjects[j];
		delete[] m_ppObjects;
	}
}

void CShader::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; j++)
	{
		m_ppObjects[j]->Animate(fTimeElapsed);
	}
}
void CShader::OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
}

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	OnPrepareRender(pd3dDeviceContext);
	//���̴� ��ü�� ��� ���� ��ü���� �������Ѵ�.
	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			m_ppObjects[j]->Render(pd3dDeviceContext);
		}
	}
}

CSceneShader::CSceneShader()
{
}

CSceneShader::~CSceneShader()
{
}

void CSceneShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
}

void CSceneShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	int xObjects = 6, yObjects = 6, zObjects = 6, i = 0;
	/*������ü ��ü�� x-��, y-��, z-������ ���� 13���� �� 2,197��(13x13x13) �����ϰ� �� ��ü�� 16�� �����Ѵ�.*/ 
		m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1) + (8 * 2);
	m_ppObjects = new CGameObject*[m_nObjects];

	//������ü �޽��� �����Ѵ�.
	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DCOLOR_XRGB(0, 0, 128));

	float fxPitch = 12.0f * 1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;
	CRotatingObject *pRotatingObject = NULL;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int z = -zObjects; z <= zObjects; z++)
			{
				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pCubeMesh);
				pRotatingObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
				m_ppObjects[i++] = pRotatingObject;
			}
		}
	}

	//�� �޽��� �����Ѵ�.
	CSphereMesh *pSphereMesh = new CSphereMesh(pd3dDevice, 12.0f, 20, 20, D3DCOLOR_XRGB(64, 0, 0));

	/*�� ��ü�� �����ϴ� ��ü�̴�. 8���� ������ǥ���� xy-��鿡�� ������ �߽����� 45�� �������� ��ġ�Ѵ�. z-������ ȸ���Ѵ�. 8���� ������ǥ���� yz-��鿡�� ������ �߽����� 45�� �������� ��ġ�Ѵ�. x-������ ȸ���Ѵ�.*/
	CRevolvingObject *pRevolvingObject = NULL;
	float fRadius = (xObjects * fxPitch * 2.0f) * 1.5f;
	for (int j = 0; j < 8; j++)
	{
		pRevolvingObject = new CRevolvingObject();
		pRevolvingObject->SetMesh(pSphereMesh);
		pRevolvingObject->SetPosition(fRadius*cosf(D3DXToRadian(45.0f)*j), fRadius*sinf(D3DXToRadian(45.0f)*j), 0.0f);
		pRevolvingObject->SetRevolutionAxis(D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		pRevolvingObject->SetRevolutionSpeed(10.0f);
		m_ppObjects[i++] = pRevolvingObject;
	}
	for (int j = 0; j < 8; j++)
	{
		pRevolvingObject = new CRevolvingObject();
		pRevolvingObject->SetMesh(pSphereMesh);
		pRevolvingObject->SetPosition(0.0f, fRadius*cosf(D3DXToRadian(45.0f)*j), fRadius*sinf(D3DXToRadian(45.0f)*j));
		pRevolvingObject->SetRevolutionAxis(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		pRevolvingObject->SetRevolutionSpeed(20.0f);
		m_ppObjects[i++] = pRevolvingObject;
	}
}

CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

void CPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);
}

void CPlayerShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	ID3D11DeviceContext *pd3dDeviceContext = NULL;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	CMesh *pAirplaneMesh = new CAirplaneMesh(pd3dDevice, 20.0f, 20.0f, 4.0f, D3DCOLOR_XRGB(0, 255, 0));
	CAirplanePlayer *pAirplanePlayer = new CAirplanePlayer(pd3dDevice);
	pAirplanePlayer->SetMesh(pAirplaneMesh);
	pAirplanePlayer->CreateShaderVariables(pd3dDevice);
	pAirplanePlayer->ChangeCamera(pd3dDevice, SPACESHIP_CAMERA, 0.0f);

	CCamera *pCamera = pAirplanePlayer->GetCamera();
	pCamera->SetViewport(pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
	pCamera->GenerateViewMatrix();

	pAirplanePlayer->SetPosition(D3DXVECTOR3(0.0f, 10.0f, -50.0f));
	m_ppObjects[0] = pAirplanePlayer;

	if (pd3dDeviceContext) pd3dDeviceContext->Release();
}

void CPlayerShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	//3��Ī ī�޶��� �� �÷��̾ �������Ѵ�.
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;
	if (nCameraMode == THIRD_PERSON_CAMERA)
	{
		CShader::Render(pd3dDeviceContext, pCamera);
	}
}
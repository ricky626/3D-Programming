#include "Shader.h"

CShader::CShader()
{
	m_pd3dVertexShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dVertexLayout = NULL;

	m_pd3dcbWorldMatrix = NULL;
	m_pd3dcbMaterial = NULL;

	m_ppObjects = NULL;
	m_nObjects = 0;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader){
		m_pd3dVertexShader->Release();
	}
	if (m_pd3dPixelShader){
		m_pd3dPixelShader->Release();
	}
	if (m_pd3dVertexLayout){
		m_pd3dVertexLayout->Release();
	}
}

void CShader::BuildObjects(ID3D11Device *pd3dDevice)
{
}

void CShader::ReleaseObjects()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();

	if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; j++) delete m_ppObjects[j];
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

void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
}

void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);
}
//
//void CShader::ReleaseShaderVariables()
//{
//}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX *pcbWorldMatrix = (VS_CB_WORLD_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial)
{
}


void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName,
	LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader,
	D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags != D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*
	파일(pszFileName)에서 쉐이더 함수(pszShaderName)을 컴파일하여 컴파일된 쉐이더 코드의 메모리주소(pd3dShaderBlob)를
	반환한다.
	*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL,
		pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob,
		&pd3dErrorBlob, NULL)))
	{
		// 컴파일 된 쉐이더 코드의 메모리를 주소에서 정점-쉐이더를 생성한다.
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(),
			pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		// 컴파일 된 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다.
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(),
			pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName,
	LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **pd3dPixelShader)
{
	HRESULT hResult;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/* 파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일 된 쉐이더 코드의 메모리주소
	(pd3dShaderBlob)를 반환한다.
	*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL,
		pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob,
		&pd3dErrorBlob, NULL)))
	{
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(),
			pd3dShaderBlob->GetBufferSize(), NULL, pd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			//객체의 물질 정보를 쉐이더 프로그램으로 전달한다.
			if (m_ppObjects[j]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_pMaterial->m_Material);
			UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
			m_ppObjects[j]->Render(pd3dDeviceContext);
		}
	}
}

CDiffusedShader::CDiffusedShader()
{
}

CDiffusedShader::~CDiffusedShader()
{
}

void CDiffusedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSDiffusedColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSDiffusedColor", "ps_4_0", &m_pd3dPixelShader);
}
void CDiffusedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}
void CDiffusedShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	CCubeMesh *pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DCOLOR_XRGB(0, 0, 128));

	int xObjects = 6, yObjects = 6, zObjects = 6, i = 0;
	m_nObjects = (xObjects + ((xObjects % 2) ? 0 : 1)) * (yObjects + ((yObjects % 2) ? 0 : 1)) * (zObjects + ((zObjects % 2) ? 0 : 1));
	m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;
	CRotatingObject *pRotatingObject = NULL;
	for (int x = -xObjects; x <= xObjects; x += 2)
	{
		for (int y = -yObjects; y <= yObjects; y += 2)
		{
			for (int z = -zObjects; z <= zObjects; z += 2)
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

	CreateShaderVariables(pd3dDevice);
}

void CDiffusedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}

void CDiffusedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}

void CDiffusedShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
}



CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}


void CPlayerShader::ReleaseObjects()
{
	CDiffusedShader::ReleaseObjects();
}

void CPlayerShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CDiffusedShader::CreateShader(pd3dDevice);
}

void CPlayerShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CDiffusedShader::CreateShaderVariables(pd3dDevice);
}

void CPlayerShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CDiffusedShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CPlayerShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CDiffusedShader::Render(pd3dDeviceContext, pCamera);
}

CIlluminatedShader::CIlluminatedShader()
{
}

CIlluminatedShader::~CIlluminatedShader()
{
}

void CIlluminatedShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}

void CIlluminatedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CIlluminatedShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL *pcbMaterial = (MATERIAL *)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CIlluminatedShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
}

void CIlluminatedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}

void CIlluminatedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}

void CIlluminatedShader::CreateShader(ID3D11Device *pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"Effect.fx", "VSLightingColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"Effect.fx", "PSLightingColor", "ps_4_0", &m_pd3dPixelShader);
}

void CIlluminatedShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	//객체들의 물질의 색상은 빨강색, 녹색, 파란색이다.
	CMaterial **ppMaterials = new CMaterial*[6];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[1] = new CMaterial();
	ppMaterials[1]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[1]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[2] = new CMaterial();
	ppMaterials[2]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 0.0f, 1.0f, 10.0f);
	ppMaterials[2]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[3] = new CMaterial();
	ppMaterials[3]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[3]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[3]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[3]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[4] = new CMaterial();
	ppMaterials[4]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
	ppMaterials[4]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[4]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[4]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[5] = new CMaterial();
	ppMaterials[5]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
	ppMaterials[5]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[5]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[5]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CCubeMeshIlluminated *pCubeXMesh = new CCubeMeshIlluminated(pd3dDevice, 20.0f, 2000.0f, 2000.0f);
	CCubeMeshIlluminated *pCubeYMesh = new CCubeMeshIlluminated(pd3dDevice, 4000.0f, 20.0f, 2000.0f);
	CCubeMeshIlluminated *pCubeZMesh = new CCubeMeshIlluminated(pd3dDevice, 4000.0f, 2000.0f, 20.0f);
	CSphereMeshIlluminated *pSphereMeshIlluminated = new CSphereMeshIlluminated(pd3dDevice, 12.0f, 40, 40);

	int i = 0;
	int tank_cnt = 10;
	int map_x_cnt = 2;	// 왼쪽, 오른쪽
	int map_y_cnt = 2;	// 위, 아래
	int map_z_cnt = 2;	// 앞, 뒤
	int wall_cnt = 3;
	int wall_rotate_cnt = 3;

	m_nObjects = map_x_cnt + map_y_cnt + map_z_cnt + wall_cnt + wall_rotate_cnt + tank_cnt ;
	m_ppObjects = new CGameObject*[m_nObjects];

	CGameObject *pMapObject = nullptr;
	D3DXVECTOR3 tank_pos[10] = {
		{ 500.0f, -990.0f + 12.0f + 3.0f, 500.0f },
		{ 1000.0f, -990.0f + 12.0f + 3.0f, 200.0f },
		{ 1000.0f, -990.0f + 12.0f + 3.0f, -300.0f },
		{ -400.0f, -990.0f + 12.0f + 3.0f, -300.0f },
		{ 200.0f, -990.0f + 12.0f + 3.0f, -700.0f },
		{ -400.0f, -990.0f + 12.0f + 3.0f, 800.0f },
		{ -1000.0f, -990.0f + 12.0f + 3.0f, 200.0f },
		{ -1500.0f, -990.0f + 12.0f + 3.0f, 300.0f },
		{ 600.0f, -990.0f + 12.0f + 3.0f, 200.0f },
		{ 200.0f, -990.0f + 12.0f + 3.0f, 200.0f }
	};

	CGameObject *tankObject = NULL;
	CCubeMeshIlluminated *body = new CCubeMeshIlluminated(pd3dDevice, 120.0f, 48.0f, 80.0f);
	for (int k = 0; k < 10; ++k)
	{
		tankObject = new CGameObject();
		tankObject->SetMesh(body);
		tankObject->SetPosition(tank_pos[k]);
		tankObject->SetMaterial(ppMaterials[(rand() % 2) + 4]);
		tankObject->Rotate(0, 0, 0);
		tankObject->m_nHeight = 80; tankObject->m_nWidth = 120;
		tankObject->SetRect();
		m_ppObjects[i++] = tankObject;
	}

	for (int j = 0; j < map_x_cnt; ++j)
	{
		pMapObject = new CGameObject();
		pMapObject->SetMesh(pCubeXMesh);
		pMapObject->SetPosition(10 + (-2000) + j * 4000, 10, -100);
		pMapObject->SetMaterial(ppMaterials[0]);
		pMapObject->m_nHeight = 2000; pMapObject->m_nWidth = 20;
		pMapObject->SetRect();
		m_ppObjects[i++] = pMapObject;
	}
	for (int j = 0; j < map_y_cnt; ++j)
	{
		pMapObject = new CGameObject();
		pMapObject->SetMesh(pCubeYMesh);
		pMapObject->SetPosition(10, 10 + (-1000) + j * 2000, -100);
		pMapObject->SetMaterial(ppMaterials[1]);
		//pMapObject->SetActive();
		pMapObject->m_nHeight = 2000; pMapObject->m_nWidth = 4000;
		pMapObject->SetRect();
		m_ppObjects[i++] = pMapObject;
	}
	for (int j = 0; j < map_z_cnt; ++j)
	{
		pMapObject = new CGameObject();
		pMapObject->SetMesh(pCubeZMesh);
		pMapObject->SetPosition(10, 10 + 10, -100 + 1000 + j*-2000);
		pMapObject->SetMaterial(ppMaterials[2]);
		//pMapObject->SetActive();
		pMapObject->m_nHeight = 20; pMapObject->m_nWidth = 4000;
		pMapObject->SetRect();
		m_ppObjects[i++] = pMapObject;
	}

	CGameObject *pWallObject = nullptr;

	D3DXVECTOR3 wall_pos[5] = {
		{ 100, -1000 + 250, 100 },
		{ -760, -1000 + 250, -150 },
		{ 1200, -1000 + 250, -400 },
	};

	D3DXVECTOR3 wall_rotate_pos[5] = {
		{ 400, -1000 + 250, -400 },
		{ -500, -1000 + 250, 150 },
		{ -1000, -1000 + 250, -390 }
	};

	CCubeMeshIlluminated *pWall = new CCubeMeshIlluminated(pd3dDevice, 60.0f, 500.0f, 400.0f);

	// 회전 되지 않은 장애물
	for (int j = 0; j < wall_cnt; ++j)
	{
		pWallObject = new CGameObject();
		pWallObject->SetMesh(pWall);
		pWallObject->SetPosition(wall_pos[j]);
		pWallObject->SetMaterial(ppMaterials[3]);
		//pWallObject->SetActive();
		pWallObject->m_nHeight = 400; pWallObject->m_nWidth = 60;
		pWallObject->SetRect();
		m_ppObjects[i++] = pWallObject;
	}
	// 회전 된 장애물
	for (int j = 0; j < wall_rotate_cnt; ++j)
	{
		pWallObject = new CGameObject();
		pWallObject->SetMesh(pWall);
		pWallObject->SetPosition(wall_rotate_pos[j]);
		pWallObject->SetMaterial(ppMaterials[3]);
		pWallObject->Rotate(0, 90, 0);
		//pWallObject->SetActive();
		pWallObject->m_nHeight = 60; pWallObject->m_nWidth = 400;
		pWallObject->SetRect();
		m_ppObjects[i++] = pWallObject;
	}

	

	m_nObjects = i;

	CreateShaderVariables(pd3dDevice);

	delete[] ppMaterials;
}

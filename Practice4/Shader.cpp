#include "stdafx.h"
#include "Shader.h"

// http://blog.naver.com/snrnsrk06/100202723546 (Effect.fx)


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
	if (m_pTerrain) delete m_pTerrain;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
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

void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	for (int j = 0; j < m_nObjects; j++)
	{
		if (m_ppObjects[j])
		{
			//��ü�� ���� ������ ���̴� ���α׷����� �����Ѵ�.
			if (m_ppObjects[j]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_pMaterial->m_Material);
			UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
			m_ppObjects[j]->Render(pd3dDeviceContext);
		}
	}
	if (m_pTerrain)
	{
		UpdateShaderVariables(pd3dDeviceContext, &m_pTerrain->m_d3dxmtxWorld);
		m_pTerrain->Render(pd3dDeviceContext, pCamera);
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

void CDiffusedShader::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];

	//��ũ ��ü, ��ž�� ����ü �޽�, ������ ���� �޽��� �����Ѵ�.
	CCylinderMesh *pGunMesh = new CCylinderMesh(pd3dDevice, 0x00, 2.0f, 7.0f, 20.0f, 1, RANDOM_COLOR);
	CCubeMesh *pTurretMesh = new CCubeMesh(pd3dDevice, 10.0f, 8.0f, 7.0f, RANDOM_COLOR);
	CCubeMesh *pBodyMesh = new CCubeMesh(pd3dDevice, 20.0f, 8.0f, 12.0f, RANDOM_COLOR);
	//CSphereMesh *pBallMesh = new CSphereMesh(pd3dDevice, 2.5f, 10, 10, D3DCOLOR_XRGB(0, 0, 128));

	//��ũ ��ü�� �����Ѵ�.
	CTankObject *pTankObject = new CTankObject();
	pTankObject->SetMesh(pBodyMesh);
	pTankObject->SetPosition(1028, 100, 1028);

	//��ž ��ü�� �����ϰ� ��ũ ��ü�� �ڽ����� �����Ѵ�. ��ž�� ��ġ�� ��ũ�� ��������� ǥ���Ѵ�.
	CTurretObject *pTurretObject = new CTurretObject();
	pTurretObject->SetMesh(pTurretMesh);
	pTurretObject->SetPosition(1028.0f, 100+(48.0f + 56.0f)*0.5f, 1028.0f);
	pTankObject->SetChild(pTurretObject);

	//���� ��ü�� ��ž ��ü�� �ڽ����� �����Ѵ�. ������ ��ġ�� ��ž�� ��������� ǥ���Ѵ�.
	CGunObject *pGunObject = new CGunObject();
	pGunObject->SetMesh(pGunMesh);
	pGunObject->SetPosition(1028.0f + -(120.0f + 60.0f)*0.5f, 100 + (48.0f + 56.0f)*0.5f, 1028.0f);
	pTurretObject->SetChild(pGunObject);

	m_ppObjects[0] = pTankObject;

	CreateShaderVariables(pd3dDevice);
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
	//��ü���� ������ ������ ������, ���, �Ķ����̴�.
	CMaterial **ppMaterials = new CMaterial*[3];
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


	CCubeMeshIlluminated *pCubeMeshIlluminated = new CCubeMeshIlluminated(pd3dDevice, 12.0f, 12.0f, 12.0f);
	CSphereMeshIlluminated *pSphereMeshIlluminated = new CSphereMeshIlluminated(pd3dDevice, 12.0f, 40, 40);

	int i = 0;
	
	m_nObjects = 1;
	m_ppObjects = new CGameObject*[m_nObjects];
	//������ Ȯ���� ������ �����̴�. x-��� z-���� 8��, y-���� 2�� Ȯ���Ѵ�.
	D3DXVECTOR3 d3dxvScale(8.0f, 2.0f, 8.0f);
	D3DXCOLOR d3dxColor(0.0f, 0.2f, 0.0f, 0.0f);
	/*������ ���� �� �̹��� ������ ����Ͽ� �����Ѵ�. 
	���� �� �̹����� ũ��� ����x����(257x257)�̰� ���� �޽��� ũ��� ����x����(17x17)�̴�. 
	���� ��ü�� ���� �������� 16��, ���� �������� 16�� ���� �޽��� ������. 
	������ �����ϴ� ���� �޽��� ������ �� 256(16x16)���� �ȴ�.*/
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);
	m_pTerrain->SetMaterial(ppMaterials[1]);

	m_ppObjects[i++] = m_pTerrain;

	CreateShaderVariables(pd3dDevice);

	delete[] ppMaterials;
}

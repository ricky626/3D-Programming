#include "Scene.h"


CScene::CScene()
{
	m_ppShaders = NULL;
	m_nShaders = 0;

	m_ppObjects = NULL;
	m_nObjects = 0;
	m_pLights = NULL;
	m_pd3dcbLights = NULL;
}


CScene::~CScene()
{

}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nShaders = 1;
	m_ppShaders = new CShader*[m_nShaders];

	//m_ppShaders[0] = new CDiffusedShader();
	//m_ppShaders[0]->CreateShader(pd3dDevice);
	//m_ppShaders[0]->BuildObjects(pd3dDevice);

	m_ppShaders[0] = new CIlluminatedShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);
}

void CScene::BuildLights(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));
	//게임 월드 전체를 비추는 주변조명을 설정한다.
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	//3개의 조명(점 광원, 스팟 광원, 방향성 광원)을 설정한다.
	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 40.0f;
	m_pLights->m_pLights[0].m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvPosition = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.001f, 0.0001f);
	m_pLights->m_pLights[1].m_bEnable = 1.0f;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 5000.0f;
	m_pLights->m_pLights[1].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	m_pLights->m_pLights[2].m_bEnable = 1.0f;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	// 중앙
	m_pLights->m_pLights[3].m_bEnable = 1.0f;
	m_pLights->m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[3].m_fRange = 4000.0f;
	m_pLights->m_pLights[3].m_d3dxcAmbient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxvPosition = D3DXVECTOR3(-300.0f, 1900.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[3].m_fFalloff = 20.0f;
	m_pLights->m_pLights[3].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[3].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	
	m_pLights->m_pLights[4].m_bEnable = 1.0f;
	m_pLights->m_pLights[4].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[4].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[4].m_d3dxcDiffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[4].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[4].m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[4].m_d3dxvPosition = D3DXVECTOR3(10, -900, -100);
	
	// 플레이어 밑
	m_pLights->m_pLights[5].m_bEnable = 1.0f;
	m_pLights->m_pLights[5].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[5].m_fRange = 4000.0f;
	m_pLights->m_pLights[5].m_d3dxcAmbient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_pLights->m_pLights[5].m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[5].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[5].m_d3dxvPosition = D3DXVECTOR3(-2000.0f, -1100 + 200, -800.0f);
	m_pLights->m_pLights[5].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[5].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[5].m_fFalloff = 100.0f;
	m_pLights->m_pLights[5].m_fPhi = (float)cos(D3DXToRadian(60.0f));
	m_pLights->m_pLights[5].m_fTheta = (float)cos(D3DXToRadian(40.0f));

	for (int i = 6; i <=7; i++){
		m_pLights->m_pLights[i].m_bEnable = 1.0f;
		m_pLights->m_pLights[i].m_nType = POINT_LIGHT;
		m_pLights->m_pLights[i].m_fRange = 4000.0f;
		m_pLights->m_pLights[i].m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcDiffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcSpecular = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxvPosition = D3DXVECTOR3(-1500.0f, 0.0f, -1000*(i-6));
		m_pLights->m_pLights[i].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		m_pLights->m_pLights[i].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
		m_pLights->m_pLights[i].m_fFalloff = 1000.0f;
		m_pLights->m_pLights[i].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pLights->m_pLights[i].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	}

	for (int i = 8; i <=9; i++){
		m_pLights->m_pLights[i].m_bEnable = 1.0f;
		m_pLights->m_pLights[i].m_nType = POINT_LIGHT;
		m_pLights->m_pLights[i].m_fRange = 4000.0f;
		m_pLights->m_pLights[i].m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcDiffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcSpecular = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, -100 + 1000+(i-8)*-2000);
		m_pLights->m_pLights[i].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		m_pLights->m_pLights[i].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
		m_pLights->m_pLights[i].m_fFalloff = 1000.0f;
		m_pLights->m_pLights[i].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pLights->m_pLights[i].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	}

	for (int i = 10; i <=11; i++){
		m_pLights->m_pLights[i].m_bEnable = 1.0f;
		m_pLights->m_pLights[i].m_nType = POINT_LIGHT;
		m_pLights->m_pLights[i].m_fRange = 4000.0f;
		m_pLights->m_pLights[i].m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcDiffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcSpecular = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxvPosition = D3DXVECTOR3(-1500+(i-10)*3000, 1000, 0.0f);
		m_pLights->m_pLights[i].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		m_pLights->m_pLights[i].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
		m_pLights->m_pLights[i].m_fFalloff = 1000.0f;
		m_pLights->m_pLights[i].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pLights->m_pLights[i].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	}
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

	for (int i = 12; i < 22; i++){
		m_pLights->m_pLights[i].m_bEnable = 1.0f;
		m_pLights->m_pLights[i].m_nType = POINT_LIGHT;
		m_pLights->m_pLights[i].m_fRange = 100.0f;
		m_pLights->m_pLights[i].m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcDiffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxcSpecular = D3DXCOLOR(10.0f, 10.0f, 10.0f, 1.0f);
		m_pLights->m_pLights[i].m_d3dxvPosition = tank_pos[i - 12];
		m_pLights->m_pLights[i].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		m_pLights->m_pLights[i].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
		m_pLights->m_pLights[i].m_fFalloff = 50.0f;
		m_pLights->m_pLights[i].m_fPhi = (float)cos(D3DXToRadian(40.0f));
		m_pLights->m_pLights[i].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	}

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CScene::ReleaseLights()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

void CScene::UpdateLights(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, m_pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::ReleaseObjects()
{
	// 쉐이더 객체 리스트의 각 객체를 소멸시키고 리스트를 소멸시킨다.
	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; ++i){
			if (m_ppShaders[i]) delete m_ppShaders[i];
		}
		delete[] m_ppShaders;
	}
	// 게임 객체 리스트의 각 객체를 반환하고 리스트를 소멸시킨다.
	if (m_ppObjects)
	{
		for (int i = 0; i < m_nObjects; ++i)
		{
			if (m_ppObjects[i]) delete m_ppObjects[i];
		}
		delete[]m_ppObjects;
	}
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::ProcessInput()
{
	return false;
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nObjects; ++j) m_ppShaders[j]->AnimateObjects(fTimeElapsed);
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	bool bIsVisible = false;
	AABB bcBoundingCube;

	if (m_pLights && m_pd3dcbLights)
	{
		/*두 번째 조명은 플레이어가 가지고 있는 손전등이다.
		그러므로 카메라의 위치가 바뀌면 조명의 위치와 방향을 카메라의 위치와 방향으로 변경한다.*/
		D3DXVECTOR3 d3dxvCameraPosition = pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);

		m_pLights->m_pLights[1].m_d3dxvPosition = d3dxvCameraPosition;
		m_pLights->m_pLights[1].m_d3dxvDirection = pCamera->GetLookVector();
		m_pLights->m_pLights[5].m_d3dxvPosition = pCamera->GetPosition();

		UpdateLights(pd3dDeviceContext);
	}
	

	for (int j = 0; j < m_nShaders; ++j)
	{
		pd3dDeviceContext->IASetInputLayout(m_ppShaders[j]->m_pd3dVertexLayout);
		pd3dDeviceContext->VSSetShader(m_ppShaders[j]->m_pd3dVertexShader, NULL, 0);
		pd3dDeviceContext->PSSetShader(m_ppShaders[j]->m_pd3dPixelShader, NULL, 0);

		for (int i = 0; i < m_ppShaders[j]->m_nObjects; ++i)
		{
			if (m_ppShaders[j]->m_ppObjects[i])
			{
				/*객체의 메쉬의 바운딩 박스(모델 좌표계)를 객체의 월드 변환 행렬로 변환하고 
				새로운 바운딩 박스를 계산한다.*/
				bcBoundingCube = m_ppShaders[j]->m_ppObjects[i]->m_pMesh->m_bcBoundingCube;
				bcBoundingCube.Transform(&m_ppShaders[j]->m_ppObjects[i]->m_d3dxmtxWorld);
				//바운딩 박스(월드 좌표계)가 카메라의 절두체에 포함되는 가를 검사하고 포함되는 경우에 렌더링한다. 
				bIsVisible = pCamera->IsInFrustum(bcBoundingCube.m_d3dxvMinimum, bcBoundingCube.m_d3dxvMaximum);
				if (bIsVisible)
				{
					if (m_ppShaders[j]->m_ppObjects[i]->m_pMaterial)
						m_ppShaders[j]->UpdateShaderVariables(pd3dDeviceContext, &m_ppShaders[j]->m_ppObjects[i]->m_pMaterial->m_Material);
					m_ppShaders[j]->UpdateShaderVariables(pd3dDeviceContext, &m_ppShaders[j]->m_ppObjects[i]->m_d3dxmtxWorld);
					m_ppShaders[j]->m_ppObjects[i]->Render(pd3dDeviceContext);
				}
			}
		}
	}	

	//for (int i = 0; i < m_nShaders; i++)
	//{
	//	m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	//}
}

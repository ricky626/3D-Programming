#include "stdafx.h"
#include "Mesh.h"
#include "Object.h"

CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nReferences = 1;
	m_pd3dRasterizerState = NULL;

	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;

}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
}

void CMesh::AddRef()
{
	m_nReferences++;
}

void CMesh::Release()
{
	m_nReferences--;
	if (m_nReferences == 0) delete this;
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer) pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	//인덱스 버퍼가 있으면 인덱스 버퍼를 디바이스 컨텍스트에 연결한다.
	if (m_pd3dIndexBuffer) pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dDeviceContext->IASetPrimitiveTopology(
		m_d3dPrimitiveTopology);
	if (m_pd3dRasterizerState) pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	// 인덱스 버퍼가 있으면 인덱스 버퍼를 사용하여 메쉬를 렌더링하고
	// 없으면 정점 버퍼만을 사용하여 메쉬를 렌더링한다.
	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nOffset);
}

void CMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
}


CTriangleMesh::CTriangleMesh(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 정점(삼각형의 꼭지점)의 색상은 시계방향 순서대로 빨간색, 녹색, 파란색으로 지정한다.
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(0.0f, 0.5f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CTriangleMesh::~CTriangleMesh(){
}

void CTriangleMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}
void CTriangleMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	//래스터라이저 단계에서 컬링(은면 제거)을 하지 않도록 래스터라이저 상태를 생성한다.
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}


CCubeMesh::CCubeMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth,
	D3DXCOLOR color) : CMesh(pd3dDevice)
{
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	//프리미티브 유형을 삼각형 스트립(Triangle Strip)으로 설정한다.
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[3] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pVertices[4] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pVertices[5] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	pVertices[6] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pVertices[7] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	m_nIndices = 18;
	WORD pIndices[18];
	pIndices[0] = 5; //5,6,4 - cw
	pIndices[1] = 6; //6,4,7 - ccw
	pIndices[2] = 4; //4,7,0 - cw
	pIndices[3] = 7; //7,0,3 - ccw
	pIndices[4] = 0; //0,3,1 - cw
	pIndices[5] = 3; //3,1,2 - ccw
	pIndices[6] = 1; //1,2,2 - cw 
	pIndices[7] = 2; //2,2,3 - ccw
	pIndices[8] = 2; //2,3,3 - cw  - Degenerated Index(2)
	pIndices[9] = 3; //3,3,7 - ccw - Degenerated Index(3)
	pIndices[10] = 3;//3,7,2 - cw  - Degenerated Index(3)
	pIndices[11] = 7;//7,2,6 - ccw
	pIndices[12] = 2;//2,6,1 - cw
	pIndices[13] = 6;//6,1,5 - ccw
	pIndices[14] = 1;//1,5,0 - cw
	pIndices[15] = 5;//5,0,4 - ccw
	pIndices[16] = 0;
	pIndices[17] = 4;

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD)* m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CCubeMesh::~CCubeMesh()
{
}

void CCubeMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	//d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}


CAirplaneMesh::CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor) : CMesh(pd3dDevice)
{
	m_nVertices = 24 * 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex pVertices[24 * 3];

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//비행기 메쉬의 정점 위치를 설정한다.
	//비행기 메쉬의 위쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	//비행기 메쉬의 아래쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	//비행기 메쉬의 오른쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	//비행기 메쉬의 뒤쪽/오른쪽 평면
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);

	//비행기 메쉬의 왼쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	//비행기 메쉬의 뒤쪽/왼쪽 평면
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, RANDOM_COLOR);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState(pd3dDevice);
}

CAirplaneMesh::~CAirplaneMesh()
{
}

void CAirplaneMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CAirplaneMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

CSphereMesh::CSphereMesh(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks, D3DXCOLOR d3dxColor) : CMesh(pd3dDevice)
{
	// 사각형의 개수는 (nSlices * nStacks)이고 사각형은 2개의 삼각형으로 구성되므로
	// 삼각형 리스트일 때 정점의 개수는  ((nSlices * nStacks) * 3 * 2)개이다.
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex *pVertices = new CDiffusedVertex[m_nVertices];
	float theta_i, theta_ii, phi_j, phi_jj, fRadius_j, fRadius_jj, y_j, y_jj;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		//180도를 nStacks 만큼 분할한다. 
		phi_j = float(D3DX_PI / nStacks) * j;
		phi_jj = float(D3DX_PI / nStacks) * (j + 1);
		//180도를 nStacks 만큼 분할하여 y 좌표와 원기둥의 반지름을 구한다.
		fRadius_j = fRadius * sinf(phi_j);
		fRadius_jj = fRadius * sinf(phi_jj);
		y_j = fRadius*cosf(phi_j);
		y_jj = fRadius*cosf(phi_jj);
		for (int i = 0; i < nSlices; i++)
		{
			//360도를 nSlices 만큼 분할하고 <그림 3>과 같이 삼각형들의 리스트를 만든다.
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			//사각형의 첫 번째 삼각형의 정점을 만든다.
			pVertices[k++] = CDiffusedVertex(fRadius_j*cosf(theta_i), y_j, fRadius_j*sinf(theta_i), d3dxColor + RANDOM_COLOR);
			pVertices[k++] = CDiffusedVertex(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i), d3dxColor + RANDOM_COLOR);
			pVertices[k++] = CDiffusedVertex(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
			//사각형의 두 번째 삼각형의 정점을 만든다.
			pVertices[k++] = CDiffusedVertex(fRadius_jj*cosf(theta_i), y_jj, fRadius_jj*sinf(theta_i), d3dxColor + RANDOM_COLOR);
			pVertices[k++] = CDiffusedVertex(fRadius_jj*cosf(theta_ii), y_jj, fRadius_jj*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
			pVertices[k++] = CDiffusedVertex(fRadius_j*cosf(theta_ii), y_j, fRadius_j*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
		}
	}

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	delete[] pVertices;

	CreateRasterizerState(pd3dDevice);
}

CSphereMesh::~CSphereMesh()
{
}

void CSphereMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CSphereMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

CCylinderMesh::CCylinderMesh(ID3D11Device *pd3dDevice, BYTE nAxis, float fRadius, int nSlices, float fHeight, int nStacks, D3DXCOLOR d3dxColor) : CMesh(pd3dDevice)
{
	//nSlices는 원통의 표면을 몇 개의 사각형으로 표현하는 가를 나타낸다.
	//nStacks는 원통의 높이를 몇 등분하는 가를 나타낸다.
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex *pVertices = new CDiffusedVertex[m_nVertices];
	float theta_i, theta_ii;
	float fSubHeight = (fHeight / nStacks), fBottom = -(fHeight * 0.5f), fb, fbb;
	for (int j = 0, k = 0; j < nStacks; j++)
	{
		fb = fBottom + (fSubHeight * j);
		fbb = fBottom + (fSubHeight * (j + 1));
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI / nSlices) * i;
			theta_ii = float(2 * D3DX_PI / nSlices) * (i + 1);
			//nAxis는 원통의 방향이다. 0x00: x-축, 0x01: y-축, 0x02: z-축
			switch (nAxis)
			{
			case 0x00: //x
				pVertices[k++] = CDiffusedVertex(fb, fRadius*sinf(theta_i), fRadius*cosf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fbb, fRadius*sinf(theta_i), fRadius*cosf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fb, fRadius*sinf(theta_ii), fRadius*cosf(theta_ii), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fb, fRadius*sinf(theta_ii), fRadius*cosf(theta_ii), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fbb, fRadius*sinf(theta_i), fRadius*cosf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fbb, fRadius*sinf(theta_ii), fRadius*cosf(theta_ii), d3dxColor + RANDOM_COLOR);
				break;
			case 0x01: //y
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fb, fRadius*sinf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fbb, fRadius*sinf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fb, fRadius*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fb, fRadius*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fbb, fRadius*sinf(theta_i), d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fbb, fRadius*sinf(theta_ii), d3dxColor + RANDOM_COLOR);
				break;
			case 0x02: //z
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fRadius*sinf(theta_i), fb, d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fRadius*sinf(theta_i), fbb, d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fRadius*sinf(theta_ii), fb, d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fRadius*sinf(theta_ii), fb, d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_i), fRadius*sinf(theta_i), fbb, d3dxColor + RANDOM_COLOR);
				pVertices[k++] = CDiffusedVertex(fRadius*cosf(theta_ii), fRadius*sinf(theta_ii), fbb, d3dxColor + RANDOM_COLOR);
				break;
			}
		}
	}

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	delete[] pVertices;

	CreateRasterizerState(pd3dDevice);
}

CCylinderMesh::~CCylinderMesh()
{
}

void CCylinderMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCylinderMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

CHeightMapGridMesh::CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor, void *pContext) : CMeshIlluminated(pd3dDevice)
{
	//격자의 교점(정점)의 개수는 (nWidth * nLength)이다.
	m_nVertices = nWidth * nLength;
	m_nStride = sizeof(CNormalVertex);
	m_nOffset = 0;
	//격자는 삼각형 스트립으로 구성한다.
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	CNormalVertex *pVertices = new CNormalVertex[m_nVertices];

	m_nWidth = nWidth;
	m_nLength = nLength; 
	m_d3dxvScale = d3dxvScale;

	float fHeight = 0.0f;
	//d3dxColor는 지형의 기본 색상이다.
	D3DXCOLOR d3dxVertexColor = d3dxColor;
		
	
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++)
		{
			fHeight = OnSetHeight(x, z, pContext);
			d3dxVertexColor = OnSetColor(x, z, pContext) + d3dxColor;
			pVertices[i++] = CNormalVertex(D3DXVECTOR3((x*m_d3dxvScale.x), fHeight, (z*m_d3dxvScale.z)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			//CDiffusedVertex(, d3dxVertexColor);
		}
	}
	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	

	m_nIndices = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
	WORD *pIndices = new WORD[m_nIndices];
	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			//홀수 번째 줄이므로(z = 0, 2, 4, ...) 인덱스의 나열 순서는 왼쪽에서 오른쪽 방향이다.
			for (int x = 0; x < nWidth; x++)
			{
				//첫 번째 줄을 제외하고 줄이 바뀔 때마다(x == 0) 첫 번째 인덱스를 추가한다.
				if ((x == 0) && (z > 0)) pIndices[j++] = (USHORT)(x + (z * nWidth));
				//아래, 위의 순서로 인덱스를 추가한다.
				pIndices[j++] = (USHORT)(x + (z * nWidth));
				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			//짝수 번째 줄이므로(z = 1, 3, 5, ...) 인덱스의 나열 순서는 오른쪽에서 왼쪽 방향이다.
			for (int x = nWidth - 1; x >= 0; x--)
			{
				//줄이 바뀔 때마다(x == (nWidth-1)) 첫 번째 인덱스를 추가한다.
				if (x == (nWidth - 1)) pIndices[j++] = (USHORT)(x + (z * nWidth));
				//아래, 위의 순서로 인덱스를 추가한다.
				pIndices[j++] = (USHORT)(x + (z * nWidth));
				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	CalculateVertexNormal((BYTE *)pVertices, pIndices);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD)* m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	delete[] pVertices;
	delete[] pIndices;

	SetRasterizerState(pd3dDevice);
}

CHeightMapGridMesh::~CHeightMapGridMesh()
{
}

void CHeightMapGridMesh::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CHeightMapGridMesh::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
}

float CHeightMapGridMesh::OnSetHeight(int x, int z, void *pContext)
{
	//높이 맵 객체의 높이 맵 이미지의 픽셀 값을 지형의 높이로 반환한다. 
	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
	int cxTerrain = pHeightMap->GetHeightMapWidth();
	float fHeight = pHeightMapImage[x + (z*cxTerrain)] * d3dxvScale.y;
	return(fHeight);
}

D3DXCOLOR CHeightMapGridMesh::OnSetColor(int x, int z, void *pContext)
{
	//조명의 방향 벡터이다. 
	D3DXVECTOR3 d3dxvLightDirection = -D3DXVECTOR3(0.650945f, -0.390567f, 0.650945f);
	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
	float fRed = 1.0f, fGreen = 0.8f, fBlue = 0.6f, fScale = 0.25f;

	fScale = D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z + 1), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z + 1), &d3dxvLightDirection);
	fScale = (fScale / 4.0f) + 0.05f;
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;
	D3DXCOLOR d3dxColor = D3DXCOLOR(fRed * fScale, fGreen * fScale, fBlue * fScale, 1.0f);
	return(d3dxColor);
}

CMeshIlluminated::CMeshIlluminated(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
}

CMeshIlluminated::~CMeshIlluminated()
{
}

void CMeshIlluminated::CalculateVertexNormal(BYTE *pVertices, WORD *pIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
		/*프리미티브가 삼각형 리스트일 때 인덱스 버퍼가 있는 경우와 없는 경우를 구분하여 정점의 법선 벡터를 계산한다. 인덱스 버퍼를 사용하지 않는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형의 법선 벡터로 계산한다. 인덱스 버퍼를 사용하는 경우 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			SetTriAngleListVertexNormal(pVertices);
		else
			SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		break;
		/*프리미티브가 삼각형 스트립일 때 각 정점의 법선 벡터는 그 정점이 포함된 삼각형들의 법선 벡터의 평균으로(더하여) 계산한다.*/
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

//인덱스 버퍼를 사용하지 않는 삼각형 리스트에 대하여 정점의 법선 벡터를 계산한다.
void CMeshIlluminated::SetTriAngleListVertexNormal(BYTE *pVertices)
{
	D3DXVECTOR3 d3dxvNormal;
	CNormalVertex *pVertex = NULL;
	/*삼각형(프리미티브)의 개수를 구하고 각 삼각형의 법선 벡터를 계산하고 삼각형을 구성하는 각 정점의 법선 벡터로 지정한다.*/
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		pVertex = (CNormalVertex *)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
	}
}

//삼각형의 세 정점을 사용하여 삼각형의 법선 벡터를 계산한다.
D3DXVECTOR3 CMeshIlluminated::CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3 *)(pVertices + (m_nStride * nIndex2)));
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}


/*프리미티브가 인덱스 버퍼를 사용하는 삼각형 리스트 또는 삼각형 스트립인 경우 정점의 법선 벡터는 그 정점을 포함하는 삼각형의 법선 벡터들의 평균으로 계산한다.*/
void CMeshIlluminated::SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip)
{
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CNormalVertex *pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;

	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];
			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CNormalVertex *)(pVertices + (j * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvSumOfNormal;
	}
}

CCubeMeshIlluminated::CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshIlluminated(pd3dDevice)
{
	int i = 0;
	m_nVertices = 8;
	m_nStride = sizeof(CNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	CNormalVertex pVertices[8];
	pVertices[0] = CNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[1] = CNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[2] = CNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[3] = CNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[4] = CNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[5] = CNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[6] = CNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[7] = CNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_nIndices = 36;

	WORD pIndices[36];
	pIndices[0] = 3; pIndices[1] = 1; pIndices[2] = 0;
	pIndices[3] = 2; pIndices[4] = 1; pIndices[5] = 3;
	pIndices[6] = 0; pIndices[7] = 5; pIndices[8] = 4;
	pIndices[9] = 1; pIndices[10] = 5; pIndices[11] = 0;
	pIndices[12] = 3; pIndices[13] = 4; pIndices[14] = 7;
	pIndices[15] = 0; pIndices[16] = 4; pIndices[17] = 3;
	pIndices[18] = 1; pIndices[19] = 6; pIndices[20] = 5;
	pIndices[21] = 2; pIndices[22] = 6; pIndices[23] = 1;
	pIndices[24] = 2; pIndices[25] = 7; pIndices[26] = 6;
	pIndices[27] = 3; pIndices[28] = 7; pIndices[29] = 2;
	pIndices[30] = 6; pIndices[31] = 4; pIndices[32] = 5;
	pIndices[33] = 7; pIndices[34] = 4; pIndices[35] = 6;

	//법선 벡터를 계산한다.
	CalculateVertexNormal((BYTE *)pVertices, pIndices);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CNormalVertex)* m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD)* m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	SetRasterizerState(pd3dDevice);
}

CCubeMeshIlluminated::~CCubeMeshIlluminated()
{
}


void CCubeMeshIlluminated::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMeshIlluminated::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMeshIlluminated::Render(pd3dDeviceContext);
}

CSphereMeshIlluminated::CSphereMeshIlluminated(ID3D11Device *pd3dDevice, float fRadius, int nSlices, int nStacks) : CMeshIlluminated(pd3dDevice)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_nStride = sizeof(CNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CNormalVertex *pVertices = new CNormalVertex[m_nVertices];
	float theta_i, theta_ii, phi_j, phi_jj;
	int k = 0;
	for (int j = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI * j) / (nStacks - 1);
		phi_jj = float(D3DX_PI * (j + 1)) / (nStacks - 1);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI * i) / (nSlices - 1);
			theta_ii = float(2 * D3DX_PI * (i + 1)) / (nSlices - 1);
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	//법선 벡터를 계산한다.
	CalculateVertexNormal((BYTE *)pVertices, NULL);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	delete[] pVertices;

	SetRasterizerState(pd3dDevice);
}

CSphereMeshIlluminated::~CSphereMeshIlluminated()
{
}

void CSphereMeshIlluminated::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CSphereMeshIlluminated::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMeshIlluminated::Render(pd3dDeviceContext);
}

void CMeshIlluminated::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}
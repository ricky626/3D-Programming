#pragma once
#include "stdafx.h"

#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

typedef struct tagAABB
{
	//바운딩 박스의 최소점과 최대점을 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

	//두 개의 바운딩 박스를 합한다.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	//바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Transform(D3DXMATRIX *pd3dxmtxTransform);
} AABB;

class CVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
public :
	CVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0, 0, 0);
	}
	CVertex(D3DXVECTOR3 d3dxvPosition){ m_d3dxvPosition = d3dxvPosition; }
	~CVertex(){}
};

class CNormalVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//조명의 영향을 계산하기 위하여 법선벡터가 필요하다.
	D3DXVECTOR3 m_d3dxvNormal;

	CNormalVertex(float x, float y, float z, float nx, float ny, float nz) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); }
	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; }
	CNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~CNormalVertex() { }
};

class CMesh
{
public:
	CMesh(ID3D11Device *pd3dDevice);

	AABB m_bcBoundingCube;
	virtual ~CMesh();
private :
	int m_nReferences;
protected :
	// 정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터를 선언한다.
	ID3D11Buffer *m_pd3dVertexBuffer;
	// 정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가 정점 버퍼의 어디에서부터 시작하는가를 나타내는 변수
	UINT m_nVertices;
	UINT m_nStride;
	UINT m_nOffset;

	/*인덱스 버퍼(인덱스의 배열)에 대한 인터페이스 포인터이다. 인덱스 버퍼는 정점 버퍼(배열)에 대한 인덱스를 가진다.*/
	ID3D11Buffer *m_pd3dIndexBuffer;
	//인덱스 버퍼가 포함하는 인덱스의 개수이다. 
	UINT m_nIndices;
	//인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스이다. 
	UINT m_nStartIndex;
	//각 인덱스에 더해질 인덱스이다. 
	int m_nBaseVertex;

	// 정점 데이터가 어떤 프리미티브를 표현하고 있는 가를 나타내는 멤버 변수를 선언한다.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
	//래스터라이저 래스터라이저 상태 객체에 대한 인터페이스 포인터를 선언한다.
	ID3D11RasterizerState *m_pd3dRasterizerState;
public :
	void AddRef();
	void Release();
	// 정점 데이터를 렌더링하는 멤버 함수를 선언한다.
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
};

class CTriangleMesh : public CMesh
{
public :
	CTriangleMesh(ID3D11Device *pd3dDevice);
	virtual ~CTriangleMesh();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
};

class CCubeMesh : public CMesh
{
public:
	//직육면체의 가로, 세로, 높이의 크기를 지정하여 직육면체 메쉬를 생성한다.
	CCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f,
		D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CCubeMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

class CDiffusedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	//생성자와 소멸자를 선언한다.
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

//이 클래스는 조명의 영향을 받는 메쉬의 베이스 클래스이다.
class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshIlluminated();
public:
	//정점이 포함된 삼각형의 법선벡터를 계산하는 함수이다.
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);
	void SetTriAngleListVertexNormal(BYTE *pVertices);
	//정점의 법선벡터의 평균을 계산하는 함수이다.
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(BYTE *pVertices, WORD *pIndices);

	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};

class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
	CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminated();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};

class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshIlluminated();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};

class CCylinderMesh : public CMesh
{
public:
	CCylinderMesh(ID3D11Device *pd3dDevice, BYTE nAxis = 0x01, float fRadius = 2.0f, int nSlices = 20, float fHeight = 2.0f, int nStacks = 1, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CCylinderMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
};

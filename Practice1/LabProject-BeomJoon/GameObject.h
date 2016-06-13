#pragma once

#define DegreeToRadian(x)	((x)*3.1415926535/180.0)

class CVertex
{
public:
	CVertex() { }
	CVertex(float x, float y, float z, DWORD dwColor = 0x00000000)
	{
		m_vPosition = D3DXVECTOR3(x, y, z); m_dwColor = dwColor;
	}

	D3DXVECTOR3				m_vPosition;
	DWORD					m_dwColor;
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

	int						m_nVertices;
	CVertex					*m_pVertices;

	void SetVertex(int nIndex, CVertex vertex);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

	int						m_nReferences;
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	int						m_nPolygons;
	CPolygon				**m_ppPolygons;

public:
	void SetPolygon(int nIndex, CPolygon *pPolygon);
};

class CGameObject
{
public:
	CGameObject(CMesh *pMesh);
	CGameObject();
	~CGameObject();

public:
	CMesh					*m_pMesh;
	D3DXMATRIX				m_mtxWorld;
	
	D3DXVECTOR3				m_vMovingDirection;
	float					m_fMovingSpeed;
	float					m_fMovingRange;

	D3DXVECTOR3				m_vRotationAxis;
	float					m_fRotationSpeed;
	int						m_flag;

public:
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }
	void SetPosition(float x, float y, float z);
	void SetPosition(D3DXVECTOR3 d3dxvPosition);

	void SetMovingDirection(D3DXVECTOR3 vMovingDirection) { D3DXVec3Normalize(&m_vMovingDirection, &vMovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetRotationAxis(D3DXVECTOR3 vRotationAxis) { D3DXVec3Normalize(&m_vRotationAxis, &vRotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(D3DXVECTOR3& vDirection, float fSpeed);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(D3DXVECTOR3& vAxis, float fAngle);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetLook();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();

	virtual void Animate();
};

class CCamera
{
public:
	CCamera();
	~CCamera();

	D3DXVECTOR3				m_d3dxvPosition;
	D3DXVECTOR3				m_d3dxvRight;
	D3DXVECTOR3				m_d3dxvUp;
	D3DXVECTOR3				m_d3dxvLook;

	D3DXMATRIX				m_mtxView;
	D3DXMATRIX				m_mtxProjection;

	int						m_xViewport;
	int						m_yViewport;
	int						m_cxViewport;
	int						m_cyViewport;

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fAspectRatio, float fFOVAngle);
	void SetLookAt(D3DXVECTOR3& vPosition, D3DXVECTOR3& d3dxvLookAt, D3DXVECTOR3& d3dxvUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	void SetViewport(int xStart, int yStart, int nWidth, int nHeight);

	void Move(D3DXVECTOR3& d3dxvShift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
};

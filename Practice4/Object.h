#pragma once

#include "Camera.h"
#include "Mesh.h"

#define DIR_FORWARD	0x01
#define DIR_BACKWARD	0x02
#define DIR_LEFT	0x04
#define DIR_RIGHT	0x08
#define DIR_UP		0x10
#define DIR_DOWN	0x20

struct MATERIAL
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular; //(r,g,b,a=power)
	D3DXCOLOR m_d3dxcEmissive;
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

	int m_nReferences;
	void AddRef();
	void Release();

	MATERIAL m_Material;
};

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	CGameObject *m_pChild;
	//��ü�� ù ��° �ڽ� ��ü�� ���� �������̴�. 
	CGameObject *m_pSibling;
	//��ü�� ���� ��ü�� ���� �������̴�. 
	CGameObject *m_pParent;
	//��ü�� �θ� ��ü�� ���� �������̴�. 

	virtual void SetChild(CGameObject *pChild);

	//��ü�� ��ġ�� �����Ѵ�.
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	//���� x-��, y-��, z-�� �������� �̵��Ѵ�.
	virtual void MoveStrafe(float fDistance = 1.0f);
	virtual void MoveUp(float fDistance = 1.0f);
	virtual void MoveForward(float fDistance = 1.0f);

	//���� x-��, y-��, z-�� �������� ȸ���Ѵ�.
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle);

	//��ü�� ��ġ, ���� x-��, y-��, z-�� ���� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetLookAt();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
private:
	int m_nReferences;
public:
	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	void SetMaterial(CMaterial *pMaterial);
	virtual void SetMesh(CMesh *pMesh);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	//���� �ӵ��� ȸ���� ���͸� �����ϴ� �Լ��̴�.
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

private:
	//���� �ӵ��� ȸ���� ���͸� ��Ÿ���� ��� ������ �����Ѵ�.
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;
};

class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject();
	virtual ~CRevolvingObject();

	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);

	// ���� �ӷ��� �����Ѵ�.
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	// ������ ���� ȸ������ �����Ѵ�.
	void SetRevolutionAxis(D3DXVECTOR3 d3dxvRevolutionAxis) { m_d3dxvRevolutionAxis = d3dxvRevolutionAxis; }

private:
	// ���� ȸ����� ȸ�� �ӷ��� ��Ÿ����.
	D3DXVECTOR3 m_d3dxvRevolutionAxis;
	float m_fRevolutionSpeed;
};

/////////////

class CHeightMap
{
private:
	//���� �� �̹��� �ȼ�(8-��Ʈ)���� ������ �迭�̴�. �� �ȼ��� 0~255�� ���� ���´�.
	BYTE *m_pHeightMapImage;
	//���� �� �̹����� ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;
	//���� ���� ������ �� �� Ȯ���Ͽ� ����� ���ΰ��� ��Ÿ���� ������ �����̴�.
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale);
	~CHeightMap(void);

	//���� �� �̹������� (x, z) ��ġ�� �ȼ� ���� ����� ������ ���̸� ��ȯ�Ѵ�.
	float GetHeight(float x, float z, bool bReverseQuad = false);
	//���� �� �̹������� (x, z) ��ġ�� ���� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor);
	~CHeightMapTerrain();

	//������ ���� ������ ����� �̹����̴�.
	CHeightMap *m_pHeightMap;

	//������ ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;

	//������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ���� ������ �����̴�.
	D3DXVECTOR3 m_d3dxvScale;
	//���� ��ü�� � ��ġ�� ������ ���� ���� ��Ÿ���� ���� ��ȯ ����̴�.
	D3DXMATRIX m_d3dxmtxWorld;

	//������ �����ϴ� ���� �޽����� �迭�� �޽����� �����̴�.
	CHeightMapGridMesh **m_ppMesh;
	int m_nMeshes;

	void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera = NULL);
};

///////////////

class CGunObject : public CGameObject
{
public:
	CGunObject();
	virtual ~CGunObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetRotationAngle(float fRotationAngle) { m_fRotationAngle = fRotationAngle; }
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }

private:
	//���� ��ü�� ���� ȸ�� ������ ��Ÿ����. ������ ��-�Ʒ� ����(y-��)���� ������ ���� �������� ȸ���Ѵ�.
	float	m_fRotationAngle;
	//���� ��ü�� ȸ�� �ӷ�(1�� ���� ȸ���ϴ� ����)�� ��Ÿ����. 
	float	m_fRotationSpeed;
};

class CTurretObject : public CGameObject
{
public:
	CTurretObject();
	virtual ~CTurretObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }

private:
	//��ž ��ü�� ȸ�� �ӷ�(1�� ���� ȸ���ϴ� ����)�� ��Ÿ����. ��ž�� y-������ 360�� ȸ���� �� �ִ�. 
	float m_fRotationSpeed;
};

class CTankObject : public CGameObject
{
public:
	CTankObject();
	virtual ~CTankObject();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxParent = NULL, CCamera *pCamera = NULL);

	void SetMovingSpeed(float fMovingSpeed) { m_fMovingSpeed = fMovingSpeed; }

private:
	//��ũ ��ü�� �̵� �ӷ�(1�� ���� �̵��ϴ� �Ÿ�)�� ��Ÿ����. 
	float m_fMovingSpeed;
};

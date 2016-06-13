#pragma once

#include "GameObject.h"

struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};

class CShader
{
public:
	CShader();
	~CShader();

	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);
	void CreateGeometryShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11GeometryShader **ppd3dGeometryShader);

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
public:
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	ID3D11PixelShader *m_pd3dPixelShader;

	ID3D11GeometryShader *m_pd3dGeometryShader;

	CGameObject **m_ppObjects;
	int m_nObjects;

	ID3D11Buffer *m_pd3dcbWorldMatrix;
	ID3D11Buffer *m_pd3dcbMaterial;
};

class CDiffusedShader : public CShader
{
public:
	CDiffusedShader();
	~CDiffusedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	//virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);//
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CPlayerShader : public CDiffusedShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	//ID3D11Buffer *m_pd3dcbWorldMatrix;
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);

	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

class CIlluminatedShader : public CShader
{
public:
	CIlluminatedShader();
	~CIlluminatedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial = NULL);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera = NULL);
};

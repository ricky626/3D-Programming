#pragma once

struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;
};

class CShader
{
private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

public:
	CShader();
	virtual ~CShader();

	//���̴� �ҽ� �ڵ� ���Ͽ��� ����-���̴��� �����ϴ� �Լ��̴�. 
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);

	//���̴� �ҽ� �ڵ� ���Ͽ��� �ȼ�-���̴��� �����ϴ� �Լ��̴�. 
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);

	//���̴��� �����ϱ� ���� ȣ���ϴ� �����Լ��̴�.
	virtual void CreateShader(ID3D11Device *pd3dDevice);

	//�������ϱ� ���� ȣ���ϴ� �����Լ��̴�.
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	//���̴� Ŭ������ ��� ���۸� �����ϰ� ��ȯ�ϴ� ��� �Լ��� �����Ѵ�. 
	virtual void CreateShaderVariables(ID3D11Device *pd3dDevice);
	virtual void ReleaseShaderVariables();
	//���̴� Ŭ������ ��� ���۸� �����ϴ� ��� �Լ��� �����Ѵ�.
	virtual void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld);

public:
	//����-���̴� �������̽� �����Ϳ� �Է�-���̾ƿ� �������̽� �����͸� �����Ѵ�.
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	//�ȼ�-���̴� �������̽� �����͸� �����Ѵ�.
	ID3D11PixelShader *m_pd3dPixelShader;

public:
	//���� ��ȯ ����� ���� ��� ���ۿ� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11Buffer *m_pd3dcbWorldMatrix;
};
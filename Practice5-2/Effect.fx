//ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ���̴� ������ �����Ѵ�(���� 0�� ���).
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
};

//���� ��ȯ ����� ���� ���̴� ������ �����Ѵ�(���� 1�� ���). 
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};

/*(����) register(b0)���� b�� �������Ͱ� ��� ���۸� ���� ���Ǵ� ���� �ǹ��Ѵ�. 0�� ���������� ��ȣ�̸� ���� ���α׷����� ��� ���۸� ����̽� ���ؽ�Ʈ�� ������ ���� ���� ��ȣ�� ��ġ�ϵ��� �ؾ� �Ѵ�.
pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
*/
//����-���̴��� ����� ���� ����ü�̴�.

// �Է� �Ķ������ ���� ��ġ ����(Pos)�� �״�� ��ȯ�Ѵ�. ������ ���� ��ȯ�� ���� �����Ƿ� �� ������ ��ġ ���ʹ� ���� ��ǥ��� ǥ���ȴ�.
/*
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	return Pos;
}

// �ȼ�-���̴��� �׻� ������ ��Ÿ���� 4���� �Ǽ�(float4)�� ��ȯ�ؾ� �Ѵ�. �Է� �Ķ���Ϳ� ������� ����� ������ ��ȯ�Ѵ�. �׷��Ƿ� �������� ����� �Ǵ� ��� �ȼ��� ������ ������� �ȴ�(����� = ������ + ���).
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);    // R=1.0f, G=1.0f, B=0.0f, A=1.0f, 
}
*/
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

/*����-���̴��̴�. ������ ��ġ ���͸� ���� ��ȯ, ī�޶� ��ȯ, ���� ��ȯ�� ������� �����Ѵ�. ���� �ﰢ���� �� ������ y-�������� ȸ���� ��Ÿ���� ��Ŀ� ���� ��ȯ�Ѵ�. �׷��Ƿ� �ﰢ���� ȸ���ϰ� �ȴ�.*/
VS_OUTPUT VS(float4 position : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(position, gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);
	return output;
}

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);    // R=1.0f, G=1.0f, B=0.0f, A=1.0f, 
}
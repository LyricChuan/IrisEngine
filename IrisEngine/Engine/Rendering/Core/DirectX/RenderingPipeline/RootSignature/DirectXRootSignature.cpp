
#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{

}

/**
ͨ��Root Signature�����ǿ��Զ��壬��Draw Call��ִ��֮ǰ��ЩResource�����󶨵�Pipeline�ϣ�
�Լ���ЩResource�ᱻӳ�䵽Shader���ĸ�register space �Լ� register slot �ϡ�
Root Signature�е�һ��Descriptor Table�����Զ���һ��������Descriptor Range ��
ÿ��Descriptor Range������һ��Descriptor Heap (CBV��SRV��UAV���Է���ͬһ��Heap��)
��������Descriptor��ÿ��Descriptor��Ӧ�洢��ͬResource Heap (Default Heap��Upload Heap ��)
�е�GPU Resource ��
*/
void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //������ǩ��
    CD3DX12_ROOT_PARAMETER RootParam[9];//�����������Ƕ�Ӧdescriptor table ���������þͿ���ÿֻ֡������Ҫ���µı�

	//ObjCBV������
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
	//DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//ViewportCBV������
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
	//DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    //Light������
    //CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;
    //DescriptorRangeLightCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
   
    //texture������
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	 InTextureNum, 3);//3��Ӧshader�ж����t3

    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//0��Ӧshader�ж����t0

    //ShadowMap ƽ�й⣬�۹�
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowMapSRV;
    DescriptorShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);//2��Ӧshader�ж����t2

    //ShadowCubeMap ���Դʹ��
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowCubeMapSRV;
    DescriptorShadowCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);//1��Ӧshader�ж����t1

    RootParam[0].InitAsConstantBufferView(0);//����
    RootParam[1].InitAsConstantBufferView(1);//�ӿ�
    RootParam[2].InitAsConstantBufferView(2);//�ƹ�
    RootParam[3].InitAsConstantBufferView(3);//��

    //t
    RootParam[4].InitAsShaderResourceView(0,1);//����

    //2D��ͼ
    RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //ShadowMap
    RootParam[7].InitAsDescriptorTable(1, &DescriptorShadowMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //ShadowCubeMap
    RootParam[8].InitAsDescriptorTable(1, &DescriptorShadowCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //������̬����
    StaticSamplerObject.BuildStaticSampler();

    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        9,
        RootParam,
        StaticSamplerObject.GetSize(),//��������
        StaticSamplerObject.GetData(),//����PTR
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//���� ���л���ǩ�� ���ú�D3D12_ROOT_SIGNATURE_DESC����Ҫͨ��D3D12SerializeRootSignature�������л�
	ComPtr<ID3DBlob> SerializeRootSignature;
	ComPtr<ID3DBlob> ErrorBlob;
	D3D12SerializeRootSignature(
		&RootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		SerializeRootSignature.GetAddressOf(),
		ErrorBlob.GetAddressOf());

	if (ErrorBlob)
	{
		Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());
	}

	//����
	GetD3dDevice()->CreateRootSignature(
		0,
		SerializeRootSignature->GetBufferPointer(),
		SerializeRootSignature->GetBufferSize(),
		IID_PPV_ARGS(&RootSignature));
}

void FDirectXRootSignature::PreDraw(float DeltaTime)
{
    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());
}

void FDirectXRootSignature::Draw(float DeltaTime)
{
}

void FDirectXRootSignature::PostDraw(float DeltaTime)
{
}

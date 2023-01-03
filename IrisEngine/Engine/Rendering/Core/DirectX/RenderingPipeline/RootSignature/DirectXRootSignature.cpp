
#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{

}

/**
通过Root Signature，我们可以定义，在Draw Call被执行之前哪些Resource将被绑定到Pipeline上，
以及这些Resource会被映射到Shader的哪个register space 以及 register slot 上。
Root Signature中的一个Descriptor Table，可以定义一个或多个的Descriptor Range 。
每个Descriptor Range定义了一个Descriptor Heap (CBV，SRV，UAV可以放在同一个Heap中)
中连续的Descriptor，每个Descriptor对应存储不同Resource Heap (Default Heap或Upload Heap 等)
中的GPU Resource 。
*/
void FDirectXRootSignature::BuildRootSignature(UINT InTextureNum)
{
    //构建根签名
    CD3DX12_ROOT_PARAMETER RootParam[9];//这里的数组就是对应descriptor table ，这样设置就可以每帧只更新需要更新的表

	//ObjCBV描述表
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
	//DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//ViewportCBV描述表
	//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
	//DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    //Light描述表
    //CD3DX12_DESCRIPTOR_RANGE DescriptorRangeLightCBV;
    //DescriptorRangeLightCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 2);
   
    //texture描述表
    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV;
    DescriptorRangeTextureSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	 InTextureNum, 3);//3对应shader中定义的t3

    CD3DX12_DESCRIPTOR_RANGE DescriptorRangeCubeMapSRV;
    DescriptorRangeCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//0对应shader中定义的t0

    //ShadowMap 平行光，聚光
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowMapSRV;
    DescriptorShadowMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);//2对应shader中定义的t2

    //ShadowCubeMap 点光源使用
    CD3DX12_DESCRIPTOR_RANGE DescriptorShadowCubeMapSRV;
    DescriptorShadowCubeMapSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);//1对应shader中定义的t1

    RootParam[0].InitAsConstantBufferView(0);//对象
    RootParam[1].InitAsConstantBufferView(1);//视口
    RootParam[2].InitAsConstantBufferView(2);//灯光
    RootParam[3].InitAsConstantBufferView(3);//雾

    //t
    RootParam[4].InitAsShaderResourceView(0,1);//材质

    //2D贴图
    RootParam[5].InitAsDescriptorTable(1, &DescriptorRangeTextureSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    RootParam[6].InitAsDescriptorTable(1, &DescriptorRangeCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
   
    //ShadowMap
    RootParam[7].InitAsDescriptorTable(1, &DescriptorShadowMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);
    
    //ShadowCubeMap
    RootParam[8].InitAsDescriptorTable(1, &DescriptorShadowCubeMapSRV, D3D12_SHADER_VISIBILITY_PIXEL);

    //构建静态采样
    StaticSamplerObject.BuildStaticSampler();

    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        9,
        RootParam,
        StaticSamplerObject.GetSize(),//采样数量
        StaticSamplerObject.GetData(),//采样PTR
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//创建 序列化根签名 设置好D3D12_ROOT_SIGNATURE_DESC后，需要通过D3D12SerializeRootSignature将其序列化
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

	//创建
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

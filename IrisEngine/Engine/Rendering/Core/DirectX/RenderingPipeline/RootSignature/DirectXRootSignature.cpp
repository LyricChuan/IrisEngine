
#include "DirectXRootSignature.h"

FDirectXRootSignature::FDirectXRootSignature()
{

}

void FDirectXRootSignature::BuildRootSignature()
{
	//构建根签名
	CD3DX12_ROOT_PARAMETER RootParam[2];

	//ObjCBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeObjCBV;
	DescriptorRangeObjCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//ViewportCBV描述表
	CD3DX12_DESCRIPTOR_RANGE DescriptorRangeViewportCBV;
	DescriptorRangeViewportCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	RootParam[0].InitAsDescriptorTable(1, &DescriptorRangeObjCBV);//第一个参数:当前有多少个描述符
	RootParam[1].InitAsDescriptorTable(1, &DescriptorRangeViewportCBV);

	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
		2,//参数个数
		RootParam,
		0,
		nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//创建 序列化根签名
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

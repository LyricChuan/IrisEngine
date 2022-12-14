#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

//Constant Buffer��Ҫ������Ӧ��descriptor��resource�󶨵�pipeline��,��constant buffer view
struct FConstantBufferViews :public IDirectXDeviceInterface_Struct
{
	void CreateConstant(UINT ObjectSize,UINT ObjectCount,bool bConstBuffer = true);//ָ����С������

	void Update(int Index, const void* InData);

	void BuildConstantBuffer(
		CD3DX12_CPU_DESCRIPTOR_HANDLE InHandle,
		UINT InConstantBufferNum,
		UINT InHandleOffset = 0);

	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return Constant->GetBuffer(); }
protected:
	shared_ptr<FRenderingResourcesUpdate> Constant;
};
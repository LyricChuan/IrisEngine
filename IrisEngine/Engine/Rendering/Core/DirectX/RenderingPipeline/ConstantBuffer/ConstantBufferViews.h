#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

//Constant Buffer需要创建对应的descriptor将resource绑定到pipeline上,即constant buffer view
struct FConstantBufferViews :public IDirectXDeviceInterface_Struct
{
	void CreateConstant(UINT ObjectSize,UINT ObjectCount,bool bConstBuffer = true);//指定大小和数量

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
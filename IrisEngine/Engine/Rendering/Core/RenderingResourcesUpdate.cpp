#include "RenderingResourcesUpdate.h"

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)
	{
		UploadBuffer->Unmap(0, NULL);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount, bool bConstBuffer)
{
	assert(InDevice);

	if (bConstBuffer)
	{
	ElementSize = GetConstantBufferByteSize(InElementSize);
	}
	else
	{
		ElementSize = InElementSize;
	}

	CD3DX12_HEAP_PROPERTIES HeapPropertie = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElementCount);
	ANALYSIS_HRESULT(InDevice->CreateCommittedResource(
		&HeapPropertie,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&UploadBuffer)));
	//upload heap�е�Resourceͨ��ID3D12Resource::Map���������Ի�ȡ��һ��ָ��subresource��Resource���Ӳ��֣���CPUָ��
	ANALYSIS_HRESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	memcpy(&Data[Index * ElementSize], InData, ElementSize);//�����ǵ�CPU���ݿ�����buffer��
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSize)
{
	/*if (!(InTypeSize % 256))
		{
			float NewFloat = (float)InTypeSize / 256.f;
			int Num = (NewFloat += 1);
			InTypeSize = Num * 256;
		}*/

	return (InTypeSize + 255) & ~255;//��Ҫ����255�ı��� ~����ȡ��
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
	return GetConstantBufferByteSize(ElementSize);
}

#include "ConstructBuffer.h"
#include "../../../Debug/EngineDebug.h"

namespace ConstructBuffer
{
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize)
	{
		ComPtr<ID3D12Resource> Buffer;
		//创建资源描述
		CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
		/*
		  通过ID3D12Device::CreateCommittedResource函数，创建一个resource以及一个隐式的heap
		  (此heap指的并不是用来存放descriptor的descriptor heap),
		  resource 会被映射到这个heap上,
		  该heap有足够大的空间包含整个resource,
		  这样GPU才能访问这些顶点等信息
		*/
		//创建默认堆
		CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
			&BufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&BufferResourceDESC,
			D3D12_RESOURCE_STATE_COMMON,
			NULL, IID_PPV_ARGS(Buffer.GetAddressOf())));

		CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
			&UpdateBufferProperties,
			D3D12_HEAP_FLAG_NONE,
			&BufferResourceDESC,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			NULL, IID_PPV_ARGS(OutTmpBuffer.GetAddressOf())));

		//设置要传输的CPU数据
		D3D12_SUBRESOURCE_DATA SubResourceData = {};
		SubResourceData.pData = InData;
		SubResourceData.RowPitch = InDataSize;
		SubResourceData.SlicePitch = SubResourceData.RowPitch;

		//标记资源为复制目标
		CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST);

		//传输前要更改resource的state
		GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

		/*
		  由于我们的顶点信息来自于CPU，那么理论上应该使用upload heap。
		  然而由于这些顶点数据是不会改变的，而GPU每帧都要读取它们，
		  那么如果我们的顶点数据很多，且在upload heap上，那么就会造成性能问题
		  (GPU访问upload heap 的带宽小于default heap)。
		  因此针对于这类数据来自于CPU，但是属于静态数据的情况，
		  往往是先创建一个resource在default heap,
		  然后在创建一个resource在upload heap 作媒介，
		  通过upload heap的resource将CPU数据传输到default heap的resource上，
		  后续GPU只需要读取default heap上的resource即可，达到最优性能。
		  d3dx12.h中为我们提功了UpdateSubresources函数来实现这样的操作
		*/
		//更新子资源，应该填充所有子资源数组
		//会先将CPU内存数据拷贝到upload heap中，然后再通过ID3D12CommandList::CopySubresourceRegion从upload heap中拷贝到default buffer中
		UpdateSubresources<1>(
			GetGraphicsCommandList().Get(),
			Buffer.Get(),
			OutTmpBuffer.Get(),
			0,//0 -> D3D12_REQ_SUBRESOURCES
			0,//0 -> D3D12_REQ_SUBRESOURCES
			1,
			&SubResourceData);

		CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ);

		return Buffer;
	}
}
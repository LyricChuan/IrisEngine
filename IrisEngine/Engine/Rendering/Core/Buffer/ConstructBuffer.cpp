#include "ConstructBuffer.h"
#include "../../../Debug/EngineDebug.h"

namespace ConstructBuffer
{
	ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* InData, UINT64 InDataSize)
	{
		ComPtr<ID3D12Resource> Buffer;
		//������Դ����
		CD3DX12_RESOURCE_DESC BufferResourceDESC = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
		/*
		  ͨ��ID3D12Device::CreateCommittedResource����������һ��resource�Լ�һ����ʽ��heap
		  (��heapָ�Ĳ������������descriptor��descriptor heap),
		  resource �ᱻӳ�䵽���heap��,
		  ��heap���㹻��Ŀռ��������resource,
		  ����GPU���ܷ�����Щ�������Ϣ
		*/
		//����Ĭ�϶�
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

		//����Ҫ�����CPU����
		D3D12_SUBRESOURCE_DATA SubResourceData = {};
		SubResourceData.pData = InData;
		SubResourceData.RowPitch = InDataSize;
		SubResourceData.SlicePitch = SubResourceData.RowPitch;

		//�����ԴΪ����Ŀ��
		CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(Buffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST);

		//����ǰҪ����resource��state
		GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

		/*
		  �������ǵĶ�����Ϣ������CPU����ô������Ӧ��ʹ��upload heap��
		  Ȼ��������Щ���������ǲ���ı�ģ���GPUÿ֡��Ҫ��ȡ���ǣ�
		  ��ô������ǵĶ������ݺܶ࣬����upload heap�ϣ���ô�ͻ������������
		  (GPU����upload heap �Ĵ���С��default heap)��
		  ����������������������CPU���������ھ�̬���ݵ������
		  �������ȴ���һ��resource��default heap,
		  Ȼ���ڴ���һ��resource��upload heap ��ý�飬
		  ͨ��upload heap��resource��CPU���ݴ��䵽default heap��resource�ϣ�
		  ����GPUֻ��Ҫ��ȡdefault heap�ϵ�resource���ɣ��ﵽ�������ܡ�
		  d3dx12.h��Ϊ�����Ṧ��UpdateSubresources������ʵ�������Ĳ���
		*/
		//��������Դ��Ӧ�������������Դ����
		//���Ƚ�CPU�ڴ����ݿ�����upload heap�У�Ȼ����ͨ��ID3D12CommandList::CopySubresourceRegion��upload heap�п�����default buffer��
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
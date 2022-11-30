#include "DirectXDescriptorHeap.h"

void FDirectXDescriptorHeap::Build(UINT InNumDescriptor)
{
    //构建CBV栈描述 
    //D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV类型的descriptor heap用来存放CBV
    //D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE用于指定是提供给shader使用的
    D3D12_DESCRIPTOR_HEAP_DESC HeapDesc;
    HeapDesc.NumDescriptors = InNumDescriptor;
    HeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    HeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    HeapDesc.NodeMask = 0;

    GetD3dDevice()->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&CBVHeap));
}

void FDirectXDescriptorHeap::PreDraw(float DeltaTime)
{
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
}

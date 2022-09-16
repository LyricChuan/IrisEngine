#pragma once
#include "../../Core/Engine.h"
#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif

struct IRenderingInterface
{
	friend class FWindowsEngine;
public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	bool operator==(const IRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

	simple_c_guid GetGuid() { return Guid; }

protected:
	//OutTmpBuffer¼´Îªupdatebuffer
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTmpBuffer, const void* Indata, UINT64 InDataSize);

protected:
	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

#if defined(_WIN32)
	FWindowsEngine* GetEngine();
#else
	FEngine* GetEngine();
#endif		

private:
	static vector<IRenderingInterface*> RenderingInterface;
	simple_c_guid Guid;
};

class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
	FRenderingResourcesUpdate();
	~FRenderingResourcesUpdate();

	void Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount);

	void Update(int Index, const void* Indata);

	UINT GetConstantBufferByteSize(UINT InTypeSize);
	UINT GetConstantBufferByteSize();

	ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); }
private:
	ComPtr<ID3D12Resource> UploadBuffer;
	UINT ElementSize;
	BYTE* Data;
};
#include "DirectXDeviceInterface.h"
#include "../Rendering/Enigne/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->Fence;
		}
	}

	return NULL;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->D3dDevice;
		}
	}

	return NULL;
}

CLightManage* IDirectXDeviceInterface::GetLightManage()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GetLightManage();
		}
	}

	return NULL;
}

CMeshManage* IDirectXDeviceInterface::GetMeshManage()
{
	return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterface::GetWorld()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GetWorld();
	}
	
	return NULL;
}
ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->GraphicsCommandList;
		}
	}

	return NULL;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandAllocator;
		}
	}

	return NULL;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CommandQueue;
		}
	}

	return NULL;
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->GetRenderingEngine()->CurrentFenceIndex;
		}
	}

	return NULL;
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
	if (CWindowsEngine* InEngine = GetEngine())
	{
		if (InEngine->GetRenderingEngine())
		{
			return InEngine->MainWindowsHandle;
		}
	}

	return HWND();
}


#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
	return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IDirectXDeviceInterfece::GetEngine()
{
	return Engine;
}
#endif


ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
	return Interface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
	return Interface.GetD3dDevice();
}

CLightManage* IDirectXDeviceInterface_Struct::GetLightManage()
{
	return Interface.GetLightManage();
}

CMeshManage* IDirectXDeviceInterface_Struct::GetMeshManage()
{
	return Interface.GetMeshManage();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld()
{
	return Interface.GetWorld();
}
ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList()
{
	return Interface.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
	return Interface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
	return Interface.GetCommandQueue();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex()
{
	return Interface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle()
{
	return Interface.GetMainWindowsHandle();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
	return Interface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterfece_Struct::GetEngine()
{
	return Interfece.GetEngine();
}
#endif
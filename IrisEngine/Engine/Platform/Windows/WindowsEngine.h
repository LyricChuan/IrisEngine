#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	FWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

private:
	bool InitWindows(FWinMainCommandParameters InParameters);

	bool InitDirect3D();

protected:
	ComPtr<IDXGIFactory4> DXGIFactory;//���� DirectX ͼ�λ����ṹ (DXGI) ����
	ComPtr<ID3D12Device> D3dDevice;//��������������������б�������С�Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Fence> Fence;//һ������ͬ�� CPU ��һ������ GPU �Ķ���

	ComPtr<ID3D12CommandQueue> CommandQueue;//����
	ComPtr<ID3D12CommandAllocator> CommandAllocator;//�洢
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//�����б�

	ComPtr<IDXGISwapChain> SwapChain;//������

	//����������Ͷ�
	ComPtr<ID3D12DescriptorHeap> RTVHeap;//��Ⱦ��Ŀ����ͼ��������
	ComPtr<ID3D12DescriptorHeap> DSVHeap;//���ģ����ͼ��������

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
	ComPtr<ID3D12Resource> DepthStencilBuffer;

protected:
	HWND MainWindowsHandle;//��window���
	UINT M4XQualityLevels;
	bool bMSAA4XEnabled;
	DXGI_FORMAT BackBufferFormat;
	DXGI_FORMAT DepthStencilFormat;

	UINT RTVDescriptorSize;
};
#endif
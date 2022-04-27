#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"


FWindowsEngine::FWindowsEngine()
	: M4XQualityLevels(0)
	, bMSAA4XEnabled(false)
	, BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}
	
}

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//日志系统初始化
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//处理命令


	Engine_Log("Engine pre initialization complete.");
	return 0;
}

int FWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	//

	if (InitWindows(InParameters))
	{

	}


	Engine_Log("Engine initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	//拿到描述size
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	HeapHandle.ptr = 0;
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		HeapHandle.ptr += RTVDescriptorSize;//从当前缓冲区偏移到下一个
	}

	Engine_Log("Engine post initialization complete.");
	return 0;
}

void FWindowsEngine::Tick()
{

}

int FWindowsEngine::PreExit()
{
	Engine_Log("Engine post exit complete.");
	return 0;
}

int FWindowsEngine::Exit()
{
	Engine_Log("Engine exit complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	Engine_Log("Engine post exit complete.");
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	//注册窗口
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);				//该对象实际占用多大内存
	WindowsClass.cbClsExtra = 0;							//是否需要额外空间
	WindowsClass.cbWndExtra = 0;							//是否需要额外内存
	WindowsClass.hbrBackground = nullptr;					//如果有设置哪就是GDI擦除
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//设置一个箭头光标
	WindowsClass.hIcon = nullptr;							//应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = NULL;							//应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;		//窗口实例
	WindowsClass.lpszClassName = L"IrisEngine";				//窗口名字
	WindowsClass.lpszMenuName = nullptr;					//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;			//怎么绘制窗口 垂直和水平重绘
	WindowsClass.lpfnWndProc = EngineWindowProc;			//消息处理函数

	//注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register windows class fail.");
		MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	}

	RECT Rect = { 0,0,FEngineRenderConfig::GetRenderConfig()->ScreenWidth,FEngineRenderConfig::GetRenderConfig()->ScreenHeight };

	//@rect 视口
	//WS_OVERLAPPEDWINDOW 视口风格
	//NULL 没有菜单
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,												//窗口额外的风格
		L"IrisEngine",										//窗口名称
		L"Iris Engine",										//显示在窗口的标题栏的名称
		WS_OVERLAPPEDWINDOW,								//窗口风格
		100, 100,											//窗口的坐标
		WindowWidth, WindowHight,							//
		NULL,												//副窗口句柄
		nullptr,											//菜单句柄
		InParameters.HInstance,								//窗口实例
		NULL												//额外参数
	);

	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//显示窗口
	ShowWindow(MainWindowsHandle, SW_SHOW);

	//窗口是脏的，刷新一下
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");

}

bool FWindowsEngine::InitDirect3D()
{
	//Debug
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

////////////////////////////////////////////////////////////////////////////////////////
	//HRESULT
	//S_OK				0x00000000
	//E_UNEXPECTED		0x8000FFFF 意外的失败
	//E_NOTIMPL			0x80004001 未实现
	//E_OUTOFMEMORY		0x8007000E 未能分配所需的内存
	//E_INVALIDARG		0x80070057 一个或多个参数无效 
	//E_NOINTERFACE		0x80004002 不支持此接口
	//E_POINTER			0x80004003 无效指针
	//E_HANDLE			0x80070006 无效句柄
	//E_ABORT			0x80004004 操作终止
	//E_FAIL			0x80004005 错误
	//E_ACCESSDENIED	0x80070005 一般的访问被拒绝错误
	//ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));//检查创建的对象是否OK
	CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory));

	/*
	D3D_FEATURE_LEVEL_9_1  目标功能级别支持Direct3D 9.1包含 shader model 2.
	D3D_FEATURE_LEVEL_9_2  目标功能级别支持Direct3D 9.2包含 shader model 2.
	D3D_FEATURE_LEVEL_9_3  目标功能级别支持Direct3D 9.3包含 shader model 3.
	D3D_FEATURE_LEVEL_10_0 目标功能级别支持Direct3D 10.0包含 shader model 4.
	D3D_FEATURE_LEVEL_10_1 目标功能级别支持Direct3D 10.1包含 shader model 4.
	D3D_FEATURE_LEVEL_11_0 目标功能级别支持Direct3D 11.0包含 shader model 5.
	*/

	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))//硬件失败了，就走软件的方式创建
	{
		//warp
		ComPtr<IDXGIAdapter> WARPAdapter;
		//ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		//ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
		DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter));
		D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	}

	//创建Fence对象 为了CPU和GPU同步作准备
	//D3D12_FENCE_FLAG_NONE 
	//D3D11_FENCE_FLAG_SHARED
	//D3D11_FENCE_FLAG_SHARED_CROSS_ADAPTER
	/*
	Fence->SetEventOnCompletion
	执行命令
	提交呈现
	Queue->Signal
	wait
	*/
	//ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
	D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));

	//初始化命令对象
////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask 指示该命令队列应在哪个GPU节点上执行
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//直接
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	//ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));
	D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue));

	ID3D12CommandAllocator Allocator();
	D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()));

	D3dDevice->CreateCommandList(
		0, //默认单个Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		CommandAllocator.Get(),//将Commandlist关联到Allocator
		NULL,//ID3D12PipelineState,NULL会默认设置一个虚拟管线状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf()));

	GraphicsCommandList->Close();

	//多重采样
////////////////////////////////////////////////////////////////////////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;

	D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels));

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	//交换链
////////////////////////////////////////////////////////////////////////////////////////
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->refreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//DXGI_USAGE_BACK_BUFFER
	//DXGI_USAGE_READ_ONLY
	//DXGI_USAGE_SHADER_INPUT
	//DXGI_USAGE_SHARED
	//DXGI_USAGE_UNORDERED_ACCESS
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或资源作为输出渲染目标。
	SwapChainDesc.OutputWindow = MainWindowsHandle;//指定windows句柄
	SwapChainDesc.Windowed = true;//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//纹理格式
	//多长采样设置
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;

	DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());

	//资源描述符
////////////////////////////////////////////////////////////////////////////////////////	
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV		//CBV常量缓冲区视图 SRV着色器资源视图 UAV无序访问视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER			//采样器视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV				//渲染目标的视图资源
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV				//深度模板的视图资源
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf()));
	
	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf()));

	return false;
}

#endif
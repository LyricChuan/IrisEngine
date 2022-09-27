#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Core/World.h"

#include "../../Rendering/Enigne/DirectX/DirectX12RenderingEngine.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"


CWindowsEngine::CWindowsEngine()
	:RenderingEngine(new CDirectX12RenderingEngine())
{
	
}

CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	//��־ϵͳ��ʼ��
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	//��������


	Engine_Log("Engine pre initialization complete.");

	//��Ⱦ�����ʼ��
	RenderingEngine->PreInit(InParameters);

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	InitWindows(InParameters);

	RenderingEngine->SetMianWindowsHandle(MainWindowsHandle);

	RenderingEngine->Init(InParameters);

	CWorld* World = CreateObject<CWorld>(new CWorld());

	Engine_Log("Engine initialization complete.");
	return 0;
}

int CWindowsEngine::PostInit()
{
	Engine_Log("Engine post initialization complete.");

	RenderingEngine->PostInit();

	for (auto& Tmp : GObjects)
	{
		Tmp->BeginInit();
	}

	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	for (auto& Tmp : GObjects)
	{
		if (Tmp->IsTick())
		{
			Tmp->Tick(DeltaTime);
		}
	}

	RenderingEngine->Tick(DeltaTime);
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine post exit complete.");
	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();


	Engine_Log("Engine post exit complete.");
	return 0;
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	//ע�ᴰ��
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);				//�ö���ʵ��ռ�ö���ڴ�
	WindowsClass.cbClsExtra = 0;							//�Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;							//�Ƿ���Ҫ�����ڴ�
	WindowsClass.hbrBackground = nullptr;					//����������ľ���GDI����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);		//����һ����ͷ���
	WindowsClass.hIcon = nullptr;							//Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = NULL;							//Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;		//����ʵ��
	WindowsClass.lpszClassName = L"IrisEngine";				//��������
	WindowsClass.lpszMenuName = nullptr;					//
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;			//��ô���ƴ��� ��ֱ��ˮƽ�ػ�
	WindowsClass.lpfnWndProc = EngineWindowProc;			//��Ϣ������

	//ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register windows class fail.");
		MessageBox(NULL, L"Register windows class fail.", L"Error", MB_OK);
	}

	RECT Rect = { 0,0,FEngineRenderConfig::GetRenderConfig()->ScreenWidth,FEngineRenderConfig::GetRenderConfig()->ScreenHeight };

	//@rect �ӿ�
	//WS_OVERLAPPEDWINDOW �ӿڷ��
	//NULL û�в˵�
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;

	MainWindowsHandle = CreateWindowEx(
		NULL,												//���ڶ���ķ��
		L"IrisEngine",										//��������
		L"Iris Engine",										//��ʾ�ڴ��ڵı�����������
		WS_OVERLAPPEDWINDOW,								//���ڷ��
		100, 100,											//���ڵ�����
		WindowWidth, WindowHight,							//
		NULL,												//�����ھ��
		nullptr,											//�˵����
		InParameters.HInstance,								//����ʵ��
		NULL												//�������
	);

	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow Failed..");
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	//��ʾ����
	ShowWindow(MainWindowsHandle, SW_SHOW);

	//��������ģ�ˢ��һ��
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");

	return true;
}

#endif
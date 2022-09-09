#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

int Init(FEngine* InEngine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCmd);
#endif

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine pre initialization error, check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine initialization error, check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine post initialization error, check the initialization problem.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

void Tick(FEngine* InEngine)
{
	float DeltaTime = 0.03f;
	InEngine->Tick(DeltaTime);
	Sleep(30);//模拟
}

int Exit(FEngine* InEngine)
{
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine pre exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[&i]Engine post exit failed.", ReturnValue);
		return ReturnValue;
	}

	return ReturnValue;
}

//hInstance 自己的实例
//prevInstance 上次的实例
//cmdLine 传递命令（如批处理传递命令来启动winmain）
//showCmd 多少条cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;
	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		//初始化
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

		MSG EngineMsg = { 0 };

		//渲染
		while (EngineMsg.message != WM_QUIT)
		{
			//PM_NOREMOVE 消息不从队列中移除
			//PM_REMOVE	  消息从队列中移除
			//PM_NOYIELD  此标志使系统不释放，等待调用程序空闲的线程
			//
			//PM_QS_INPUT 处理鼠标和键盘消息
			//PM_QS_PAINT 处理画图消息
			//PM_QS_POSTMESSAGE 处理所有被寄送的消息，包括计时器和热键
			//PM_QS_SENDMESSAGE	处理所有发送消息
			if (PeekMessage(&EngineMsg,0,0,0,PM_REMOVE))//从消息队列中提取消息，如果不是WM_QUIT就REMOVE
			{
				TranslateMessage(&EngineMsg);//翻译成字符串
				DispatchMessage(&EngineMsg);//分发消息
			}
			else
			{
				Tick(Engine);
			}
		}

		//退出
		ReturnValue = Exit(Engine);

		ReturnValue = 0;//成功
	}
	else
	{
		ReturnValue = 1;
	}

	return ReturnValue;
}
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
	Sleep(30);//ģ��
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

//hInstance �Լ���ʵ��
//prevInstance �ϴε�ʵ��
//cmdLine ���������������������������winmain��
//showCmd ������cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;
	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		//��ʼ��
		Init(Engine, hInstance, prevInstance, cmdLine, showCmd);

		//��Ⱦ
		while (true)
		{
			Tick(Engine);
		}

		//�˳�
		ReturnValue = Exit(Engine);

		ReturnValue = 0;//�ɹ�
	}
	else
	{
		ReturnValue = 1;
	}

	return ReturnValue;
}
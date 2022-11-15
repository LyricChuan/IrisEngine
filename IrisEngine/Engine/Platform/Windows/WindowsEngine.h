#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Enigne/Core/RenderingEngine.h"

class CDirectXRenderingEngine;
class CWorld;
class CMeshManage;
class CWindowsEngine :public CEngine
{
	friend class IDirectXDeviceInterface;
public:
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

	CDirectXRenderingEngine* GetRenderingEngine() { return RenderingEngine; }
public:
	CMeshManage* GetMeshManage();
	CWorld* GetWorld() { return World; }
public:

	bool InitWindows(FWinMainCommandParameters InParameters);

protected:
	HWND MainWindowsHandle;//��windows���

protected:
	CDirectXRenderingEngine* RenderingEngine;
	CWorld* World;
};
#endif
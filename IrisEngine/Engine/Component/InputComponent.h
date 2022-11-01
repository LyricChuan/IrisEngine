#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyBoardInforDelegate, void, const FInputKey&);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void,int,int);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int,float);

class CInputComponent : public CComponent
{
public:
	//这里成员变量提供的代理是为更上层业务提供响应的，如camera;
	//而InputComponent自身的消息来源于input.h中提供的输入代理注册
	CVARIABLE()
	FCaptureKeyBoardInforDelegate CaptureKeyBoardInforDelegate;

	CVARIABLE(xxx,aaaa)
	FCaptureOnMouseDelegate OnMouseButtonDownDelegate;

	CVARIABLE(aaa,vvv)
	FCaptureOnMouseDelegate OnMouseButtonUpDelegate;

	CVARIABLE(xxx,www)
	FCaptureOnMouseDelegate OnMouseMoveDelegate;

	CVARIABLE(xxx, www)
	FCaptureOnMouseWheelDelegate OnMouseWheelDelegate;
public:
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	virtual void OnMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonUp(int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y,float InDelta);
};
#pragma once
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyBoardInforDelegate, void, const FInputKey&);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseDelegate, void,int,int);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureOnMouseWheelDelegate, void, int, int,float);

class CInputComponent : public CComponent
{
public:
	//�����Ա�����ṩ�Ĵ�����Ϊ���ϲ�ҵ���ṩ��Ӧ�ģ���camera;
	//��InputComponent��������Ϣ��Դ��input.h���ṩ���������ע��
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
#pragma once
#include "Core/Component.h"
#include "../EngineMinimal.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyBoardInforDelegate, void, const FInputKey&);

class CInputComponent : public CComponent
{
public:
	CVARIABLE()
	FCaptureKeyBoardInforDelegate CaptureKeyBoardInforDelegate;

public:
	virtual void BeginInit();
	virtual void Tick(float DeltaTime);
};
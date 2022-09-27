#pragma once

#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
	CVARIABLE()
	CTransformationComponent* TransformationComponent;
	
	CVARIABLE()
	CInputComponent* InputComponent;
public:
	CCamera();

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	virtual void ExecuteKeyBoard(const FInputKey& InputKey);

public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE	CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
};
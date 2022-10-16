#pragma once
#include "Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"
#include "CameraType.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class CCamera :public CCoreMinimalObject
	,public FViewport
	,public IDirectXDeviceInterface
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

	virtual void BuildViewMatrix(float DeltaTime);
public:
	virtual void OnMouseButtonDown(int X, int Y);
	virtual void OnMouseButtonUp(int X, int Y);
	virtual void OnMouseMove(int X, int Y);
	virtual void OnMouseWheel(int X, int Y, float InDelta);

	virtual void MoveForward(float InValue);
	virtual void MoveRight(float InValue);
protected:
	void RotateAroundYAxis(float InRotateDegrees);
	void RotateAroundZAxis(float InRotateDegrees);
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
	FORCEINLINE	CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

protected:
	POINT LastMousePosition;
	bool bLeftMouseDown;

	float MouseSensitivity;
	ECmeraType CmeraType;

	//ÇòÃæ×ø±ê
	float Radius;
	float A;//Theta
	float B;//
};
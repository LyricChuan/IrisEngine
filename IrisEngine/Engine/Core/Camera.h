#pragma once
#include "Viewport/Viewport.h"
#include "../Actor/Core/ActorObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"

enum ECmeraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class GCamera :public GActorObject
	,public FViewport
	,public IDirectXDeviceInterface
{
	CVARIABLE()
	CInputComponent* InputComponent;
public:
	GCamera();

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
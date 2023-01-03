#pragma once
#include "Viewport/Viewport.h"
#include "Viewport/ClientViewport.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"

enum ECmeraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;

class GCamera :public GClientViewport
	,public IDirectXDeviceInterface
{
	typedef GClientViewport Super;

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

public:
	virtual void OnClickedScreen(int X, int Y);

protected:
	void RotateAroundXAxis(float InRotateDegrees);
	void RotateAroundYAxis(float InRotateDegrees);
public:
	FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }

protected:
	POINT LastMousePosition;
	bool bLeftMouseDown;

	float MouseSensitivity;
	ECmeraType CmeraType;

	//��������
	float Radius;
	float A;//Theta
	float B;//

	class CMeshComponent* SphereMesh;
};
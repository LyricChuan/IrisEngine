#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void CCamera::BeginInit()
{
	InputComponent->CaptureKeyBoardInforDelegate.Bind(this, &CCamera::ExecuteKeyBoard);
}

void CCamera::Tick(float DeltaTime)
{

}

void CCamera::ExecuteKeyBoard(const FInputKey& InputKey)
{

}

#include "InputComponent.h"

void CInputComponent::BeginInit()
{

}

void CInputComponent::Tick(float DeltaTime)
{
	if (CaptureKeyBoardInforDelegate.IsBound())
	{
		FInputKey InputKey;
		CaptureKeyBoardInforDelegate.Execute(InputKey);
	}
}


#pragma once
#include "../../EngineMinimal.h"

enum EPressState
{
	PRESS,
	RELEASE,
};

struct FInputKey
{
	FInputKey();

	string KeyName;
	EPressState PressState;
};
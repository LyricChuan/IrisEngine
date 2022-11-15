#pragma once
#include "../../../EngineMinimal.h"

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;// 4
	int Padding1;
	int Padding2;
	int Padding3;

	XMFLOAT4 BaseColor;
	float	 Roughness;
	XMFLOAT4X4 TransformInformation;
};

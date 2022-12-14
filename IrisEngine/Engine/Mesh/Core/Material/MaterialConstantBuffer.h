#pragma once
#include "../../../EngineMinimal.h"

struct FMaterialConstantBuffer
{
	FMaterialConstantBuffer();

	int MaterialType;// 4
	float	 Roughness;
	int BaseColorIndex;
	int NormalIndex;

	int SpecularIndex;
	int Padding1;
	int Padding2;
	int Padding3;

	XMFLOAT4 BaseColor;
	XMFLOAT3 SpecularColor;
	int Padding4;
	XMFLOAT3 FresnelF0;
	float Transparency;
	XMFLOAT4X4 TransformInformation;
};

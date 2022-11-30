#pragma once
#include "MeshType.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;	
	XMFLOAT4X4 TextureTransformation;
	UINT MaterialIndex;
	UINT Padding1;
	UINT Padding2;
	UINT Padding3;
};

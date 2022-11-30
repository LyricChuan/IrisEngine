#pragma once
#include "Core/StaticMeshComponent.h"

class CBoxMeshComponent :public CStaticMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);

	void BuildKey(size_t& OutHashKey,float InHeight, float InWidth, float InDepth);
};

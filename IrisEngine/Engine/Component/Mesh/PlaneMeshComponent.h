#pragma once
#include "Core/StaticMeshComponent.h"

class CPlaneMeshComponent :public CStaticMeshComponent
{
public:
	CPlaneMeshComponent();
	
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);

	void BuildKey(size_t& OutHashKey, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};

#pragma once
#include "Core/StaticMeshComponent.h"

class CTorusMeshComponent :public CStaticMeshComponent
{
public:
	CTorusMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

	void BuildKey(size_t &OutHashKey, float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};

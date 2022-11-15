#pragma once
#include "StaticMeshComponent.h"

class CSphereMeshComponent :public CStaticMeshComponent
{
public:
	CSphereMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};

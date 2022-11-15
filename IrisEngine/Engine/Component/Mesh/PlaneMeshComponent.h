#pragma once
#include "StaticMeshComponent.h"

class CPlaneMeshComponent :public CStaticMeshComponent
{
public:
	CPlaneMeshComponent();
	
	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};

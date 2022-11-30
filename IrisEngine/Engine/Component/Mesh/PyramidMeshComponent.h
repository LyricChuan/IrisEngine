#pragma once
#include "Core/StaticMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

class CPyramidMeshComponent :public CStaticMeshComponent
{
public:
	CPyramidMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide,uint32_t InSize = 1);

	void BuildKey(size_t& OutHashKey, EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize = 1);
};

#pragma once
#include "StaticMeshComponent.h"

class CBoxMeshComponent :public CStaticMeshComponent
{
public:
	CBoxMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
};

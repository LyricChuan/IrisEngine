#pragma once
#include "Core/StaticMeshComponent.h"

class CCustomMeshComponent :public CStaticMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, string& InPath);

	static bool LoadObjFromBuff(char* InBuff, uint32_t InBuffSize, FMeshRenderingData& MeshData);

	void BuildKey(size_t& OutHashKey,std::string& InPath);
};

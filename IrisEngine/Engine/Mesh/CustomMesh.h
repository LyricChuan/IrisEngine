#pragma once

#include "Core/Mesh.h"

class CCustomMesh :public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	virtual void Draw(float DeltaTime);

	void CreateMesh(FMeshRenderingData& MeshData, string &InPath);

	static bool LoadObjFromBuff(char *InBuff,uint32_t InBuffSize, FMeshRenderingData &MeshData);
};
#pragma once

#include "Core/Mesh.h"

class GSphereMesh :public GMesh
{
	typedef GMesh Super;
public:
	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
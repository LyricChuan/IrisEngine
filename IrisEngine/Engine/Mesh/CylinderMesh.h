#pragma once

#include "Core/Mesh.h"

class GCylinderMesh :public GMesh
{
	typedef GMesh Super;
public:
	GCylinderMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(
		float InTopRadius, 
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision, 
		uint32_t InHeightSubdivision);
};
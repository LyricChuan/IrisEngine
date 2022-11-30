#pragma once

#include "Core/Mesh.h"

class GBoxMesh :public GMesh
{
	typedef GMesh Super;
public:
	GBoxMesh();

	virtual void Init();

	virtual void Draw(float DeltaTime);

	void CreateMesh(float InHeight, float InWidth, float InDepth);
};
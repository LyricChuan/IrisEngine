#pragma once
#include "../../Mesh/SphereMesh.h"

class GSky :public GSphereMesh
{
	typedef GSphereMesh Super;

public:
	GSky();

public:
	virtual void Tick(float DeltaTime);
};
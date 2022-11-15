#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	SetLightComponent(CreateObject<CParallelLightComponent>(new CParallelLightComponent()));
}

void GParallelLight::Tick(float DeltaTime)
{
	fvector_3d v3 = GetRotation();

	v3.x += DeltaTime * 40.f;
	v3.y += DeltaTime * 40.f;
	//v3.z += DeltaTime * 100.f;

	SetRotation(v3);
}
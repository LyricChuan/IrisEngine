#pragma once
#include "EngineMinimal.h"

class CComponent;
class GActorObject;

struct FCollisionResult
{
	FCollisionResult();

	bool bHit;//�Ƿ�����

	fvector_3d Location;
	fvector_3d Normal;

	float Distance;
	float Time;

	CComponent* Component;
	GActorObject *Actor;
};
#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;
//G -> Game
class GActorObject :public CCoreMinimalObject
{
	CVARIABLE()
	CTransformationComponent* TransformationComponent;

public:
	GActorObject();

public:
	FORCEINLINE CTransformationComponent* GetTransformationComponent() const {return TransformationComponent;}
public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const fvector_3d& InNewScale);

public:
	XMFLOAT3& GetPosition();
	fvector_3d GetRotation()const;
	fvector_3d GetScale()const;

	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();


};
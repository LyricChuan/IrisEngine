#pragma once
#include "Core/Component.h"

class CTransformationComponent : public CComponent
{
public:
	CTransformationComponent();

	void SetPosition(const XMFLOAT3 &InNewPosition);
	void SetForwardVector(const XMFLOAT3& InForwardVector);
	void SetRightVector(const XMFLOAT3& InRightVector);
	void SetUpVector(const XMFLOAT3& InUpVector);

public:
	XMFLOAT3 &GetPosition() { return Position; }
	XMFLOAT3 &GetForwardVector() { return ForwardVector; }
	XMFLOAT3 &GetRightVector() { return RightVector; }
	XMFLOAT3 &GetUpVector() { return UpVector; }

public:
	//½ÃÕýUp right look(fd) ·µ»Ø pos
	void CorrectionVector(); 

	fvector_3d GetCorrectionPosition();
	void GetCorrectionPosition(fvector_3d& InPos3);
private:
	XMFLOAT3 Position;

	XMFLOAT3 ForwardVector;
	XMFLOAT3 RightVector;
	XMFLOAT3 UpVector;
};


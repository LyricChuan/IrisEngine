#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
	: Position(0.f,0.f,0.f)
	,Rotation(0.f, 0.f, 0.f)
	,Scale(1.f,1.f, 1.f)
	,ForwardVector(0.f,0.f,1.f)
	,RightVector(1.f,0.f,0.f)
	, UpVector(0.f,1.f,0.f)

{

}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
	Position = InNewPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
	float RollRadians = XMConvertToRadians(InNewRotation.z);
	float PithRadians = XMConvertToRadians(InNewRotation.x);
	float YawRadians = XMConvertToRadians(InNewRotation.y);
	
	//旋转矩阵-规定先Roll，再Pitch，最后Yaw
	XMMATRIX RotationRollPitchYawMatrix = XMMatrixRotationRollPitchYaw(
		PithRadians, YawRadians,RollRadians);

	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&UpVector, XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationRollPitchYawMatrix));
	XMStoreFloat3(&ForwardVector, XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationRollPitchYawMatrix));
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
	Scale.x = InNewScale.x;
	Scale.y = InNewScale.y;
	Scale.z = InNewScale.z;
}

void CTransformationComponent::SetForwardVector(const XMFLOAT3& InForwardVector)
{

	ForwardVector = InForwardVector;

}

void CTransformationComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
	RightVector = InRightVector;

}

void CTransformationComponent::SetUpVector(const XMFLOAT3& InUpVector)
{
	UpVector = InUpVector;
}

void CTransformationComponent::CorrectionVector()
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	Forward = XMVector3Normalize(Forward);
	Up = XMVector3Normalize(XMVector3Cross(Forward, Right));//先叉乘确定垂直的UP

	Right = XMVector3Cross(Up, Forward);//再叉乘确定垂直的RIGHT

	XMStoreFloat3(&RightVector, Right);//重新赋值回去
	XMStoreFloat3(&UpVector, Up);//重新赋值回去
	XMStoreFloat3(&ForwardVector, Forward);//重新赋值回去
}

fvector_3d CTransformationComponent::GetCorrectionPosition()
{
	fvector_3d F3D;
	GetCorrectionPosition(F3D);

	return F3D;
}

void CTransformationComponent::GetCorrectionPosition(fvector_3d& InPos3)
{
	XMVECTOR Right = XMLoadFloat3(&RightVector);
	XMVECTOR Up = XMLoadFloat3(&UpVector);
	XMVECTOR Forward = XMLoadFloat3(&ForwardVector);

	XMVECTOR NewPosition = XMLoadFloat3(&Position);

	InPos3.x = -XMVectorGetX(XMVector3Dot(NewPosition, Right));
	InPos3.y = -XMVectorGetX(XMVector3Dot(NewPosition, Up));
	InPos3.z = -XMVectorGetX(XMVector3Dot(NewPosition, Forward));
}

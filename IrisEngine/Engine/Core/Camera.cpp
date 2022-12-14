#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "CameraType.h"

GCamera::GCamera()
	:Super()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	
	MouseSensitivity = 0.7f;
	CmeraType = ECmeraType::CameraRoaming;

	Radius = 10.f;
	A = XM_PI;//
	B = XM_PI;
}

void GCamera::BeginInit()
{
	//初始化我们的投影矩阵
	ViewportInit();

	InputComponent->CaptureKeyBoardInforDelegate.Bind(this, &GCamera::ExecuteKeyBoard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void GCamera::ExecuteKeyBoard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W")
	{
		MoveForward(1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);

		SetDirty(true);
	}
	else if (InputKey.KeyName == "Q")
	{
		CmeraType = ECmeraType::ObservationObject;

		SetDirty(true);
	}
	else  if (InputKey.KeyName == "E")
	{
		CmeraType = ECmeraType::CameraRoaming;

		SetDirty(true);
	}
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
	switch (CmeraType)
	{
		case CameraRoaming:
		{
			Super::BuildViewMatrix(DeltaTime);
			break;
		}
		case ObservationObject:
		{
			XMFLOAT3 &CameraPos = GetTransformationComponent()->GetPosition();

			CameraPos.x = Radius * sinf(B) * cosf(A);
			CameraPos.z = Radius * sinf(B) * sinf(A);
			CameraPos.y = Radius * cosf(B);

			XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
			XMVECTOR ViewTarget = XMVectorZero();
			XMVECTOR ViewUp = XMVectorSet(0.f, 1.0f, 0.f, 0.f);

			XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
			XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

			break;
		}
	}
}

void GCamera::OnMouseButtonDown(int X, int Y)
{
	bLeftMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMainWindowsHandle());

	SetDirty(true);
}

void GCamera::OnMouseButtonUp(int X, int Y)
{
	bLeftMouseDown = false;

	ReleaseCapture();

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetDirty(true);
}

void GCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)
	{
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MouseSensitivity);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MouseSensitivity);
		
		switch (CmeraType)
		{
			case CameraRoaming:
			{
				RotateAroundXAxis(YRadians);
				RotateAroundYAxis(XRadians);
				break;
			}	
			case ObservationObject:
			{
				A += (-XRadians);
				B += YRadians;

				A = math_libray::Clamp(A, 0.f, XM_2PI * 2.f);
				break;
			}
		}
	}

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetDirty(true);
}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CmeraType == ECmeraType::ObservationObject)
	{
		Radius += (InDelta / 100.f);

		//限制在一定的范围内
		Radius = math_libray::Clamp(Radius,7.f,40.f);
	}

	SetDirty(true);
}

void GCamera::MoveForward(float InValue)
{
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		XMFLOAT3 AT3ForwardVector = GetTransformationComponent()->GetForwardVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

	XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));//赋值回去
	GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::MoveRight(float InValue)
{
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = GetTransformationComponent()->GetPosition();
		XMFLOAT3 AT3RightVector = GetTransformationComponent()->GetRightVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		GetTransformationComponent()->SetPosition(AT3Position);
	}
}

void GCamera::RotateAroundXAxis(float InRotateDegrees)
{
	//拿到相机的方向
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUpVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于Y的旋转矩阵
	XMMATRIX RotationY = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()),InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	//XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}

void GCamera::RotateAroundYAxis(float InRotateDegrees)
{
	//拿到相机的方向
	XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
	XMFLOAT3 UPVector = GetTransformationComponent()->GetUpVector();
	XMFLOAT3 ForwardVector = GetTransformationComponent()->GetForwardVector();

	//拿到关于Z的旋转矩阵
	XMMATRIX RotationZ = XMMatrixRotationY(InRotateDegrees);

	//计算各个方向和按照Z轴旋转后的最终效果
	XMStoreFloat3(&GetTransformationComponent()->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationZ));
	XMStoreFloat3(&GetTransformationComponent()->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
}

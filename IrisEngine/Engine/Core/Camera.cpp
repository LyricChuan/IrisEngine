#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"
#include "CameraType.h"

CCamera::CCamera()
{
	InputComponent = CreateObject<CInputComponent>(new CInputComponent());
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

	MouseSensitivity = 0.7f;
	CmeraType = ECmeraType::CameraRoaming;

	Radius = 10.f;
	A = XM_PI;//
	B = XM_PI;
}

void CCamera::BeginInit()
{
	//��ʼ�����ǵ�ͶӰ����
	ViewportInit();

	InputComponent->CaptureKeyBoardInforDelegate.Bind(this, &CCamera::ExecuteKeyBoard);
	InputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
	InputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
	InputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
	InputComponent->OnMouseWheelDelegate.Bind(this, &CCamera::OnMouseWheel);
}

void CCamera::Tick(float DeltaTime)
{
	BuildViewMatrix(DeltaTime);
}

void CCamera::ExecuteKeyBoard(const FInputKey& InputKey)
{
	if (InputKey.KeyName == "W")
	{
		MoveForward(1.f);
	}
	else if (InputKey.KeyName == "S")
	{
		MoveForward(-1.f);
	}
	else if (InputKey.KeyName == "A")
	{
		MoveRight(-1.f);
	}
	else if (InputKey.KeyName == "D")
	{
		MoveRight(1.f);
	}
	else if (InputKey.KeyName == "Q")
	{
		CmeraType = ECmeraType::ObservationObject;
	}
	else  if (InputKey.KeyName == "E")
	{
		CmeraType = ECmeraType::CameraRoaming;
	}
}

void CCamera::BuildViewMatrix(float DeltaTime)
{
	switch (CmeraType)
	{
		case CameraRoaming:
		{
			//����ͽ�����
			TransformationComponent->CorrectionVector();

	//������������ƶ���ͼ
	fvector_3d V3;//��ȡ�������λ�ã��Ա���ת���������
	TransformationComponent->GetCorrectionPosition(V3);

	//����Viewmatrix ��ǰ�õ�����������
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

			ViewMatrix = {
				RightVector.x,	UpVector.x,	ForwardVector.x,	0.f,
				RightVector.y,	UpVector.y,	ForwardVector.y,	0.f,
				RightVector.z,	UpVector.z,	ForwardVector.z,	0.f,
				V3.x,			V3.y,		V3.z,				1.f };

			break;
		}
		case ObservationObject:
		{
			XMFLOAT3 &CameraPos = TransformationComponent->GetPosition();

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

void CCamera::OnMouseButtonDown(int X, int Y)
{
	bLeftMouseDown = true;

	LastMousePosition.x = X;
	LastMousePosition.y = Y;

	SetCapture(GetMainWindowsHandle());
}

void CCamera::OnMouseButtonUp(int X, int Y)
{
	bLeftMouseDown = false;

	ReleaseCapture();

	LastMousePosition.x = X;
	LastMousePosition.y = Y;
}

void CCamera::OnMouseMove(int X, int Y)
{
	if (bLeftMouseDown)
	{
		float XRadians = XMConvertToRadians((float)(X - LastMousePosition.x) * MouseSensitivity);
		float YRadians = XMConvertToRadians((float)(Y - LastMousePosition.y) * MouseSensitivity);
		
		switch (CmeraType)
		{
			case CameraRoaming:
			{
				RotateAroundYAxis(YRadians);
				RotateAroundZAxis(XRadians);
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
}

void CCamera::OnMouseWheel(int X, int Y, float InDelta)
{
	if (CmeraType == ECmeraType::ObservationObject)
	{
		Radius += (InDelta / 100.f);

		//������һ���ķ�Χ��
		Radius = math_libray::Clamp(Radius,7.f,40.f);
	}
}

void CCamera::MoveForward(float InValue)
{
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		XMFLOAT3 AT3ForwardVector = TransformationComponent->GetForwardVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR Forward = XMLoadFloat3(&AT3ForwardVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

	XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Forward, Position));//��ֵ��ȥ
	TransformationComponent->SetPosition(AT3Position);
	}
}

void CCamera::MoveRight(float InValue)
{
	if (CmeraType == ECmeraType::CameraRoaming)
	{
		XMFLOAT3 AT3Position = TransformationComponent->GetPosition();
		XMFLOAT3 AT3RightVector = TransformationComponent->GetRightVector();

		XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.f);
		XMVECTOR Right = XMLoadFloat3(&AT3RightVector);
		XMVECTOR Position = XMLoadFloat3(&AT3Position);

		XMStoreFloat3(&AT3Position, XMVectorMultiplyAdd(AmountMovement, Right, Position));
		TransformationComponent->SetPosition(AT3Position);
	}
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
	//�õ�����ķ���
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UPVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	//�õ�����Y����ת����
	XMMATRIX RotationY = XMMatrixRotationAxis(XMLoadFloat3(&TransformationComponent->GetRightVector()),InRotateDegrees);

	//�����������Ͱ���Z����ת�������Ч��
	//XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
	XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationY));
	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationY));
}

void CCamera::RotateAroundZAxis(float InRotateDegrees)
{
	//�õ�����ķ���
	XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
	XMFLOAT3 UPVector = TransformationComponent->GetUpVector();
	XMFLOAT3 ForwardVector = TransformationComponent->GetForwardVector();

	//�õ�����Z����ת����
	XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

	//�����������Ͱ���Z����ת�������Ч��
	XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
	XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UPVector), RotationZ));
	XMStoreFloat3(&TransformationComponent->GetForwardVector(), XMVector3TransformNormal(XMLoadFloat3(&ForwardVector), RotationZ));
}

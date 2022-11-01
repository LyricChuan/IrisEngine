#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

//�ṩ��Ⱦ���ݵĽӿ�
struct FDirectXRootSignature :public IDirectXDeviceInterface_Struct
{
	FDirectXRootSignature();

	void BuildRootSignature();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }
private:
	ComPtr<ID3D12RootSignature> RootSignature;
};
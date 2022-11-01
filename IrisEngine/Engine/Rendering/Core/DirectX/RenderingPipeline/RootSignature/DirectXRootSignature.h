#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

//提供渲染内容的接口
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
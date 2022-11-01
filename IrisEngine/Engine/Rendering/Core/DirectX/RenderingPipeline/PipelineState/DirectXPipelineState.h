#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"

//�ṩ��Ⱦ���ݵĽӿ�
struct FDirectXPipelineState :public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	void ResetGPSDesc();

	//�����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);
	
	//�󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* InRootSignature);

	//�󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	//BuildPSO
	void Build();
private:
	ComPtr<ID3D12PipelineState> PSO;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
};
#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"

struct FGeometry :public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	bool bRenderingDataExistence(CMeshComponent* InKey);
	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	//构建模型
	void Build();

	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); }

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	//给CPU使用的资源
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	//给GPU使用的资源
	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	FMeshRenderingData MeshRenderingData;

	//DescribeMeshRenderingData
	vector<FRenderingData> DescribeMeshRenderingData;
};

//提供渲染内容的接口
struct FGeometryMap :public IDirectXDeviceInterface_Struct
{
	FGeometryMap();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	//构建模型
	void Build();

	//描述堆
	void BuildDescriptorHeap();

	//构建Mesh常量缓冲区
	void BuildMeshConstantBuffer();

	//构建Material常量缓冲区
	void BuildMaterialConstantBuffer();

	//构建Material常量缓冲区
	void BuildLightConstantBuffer();

	//该接口会有变化
	UINT GetDrawMeshObjectNumber();

	//该接口会有变化
	UINT GetDrawMaterialObjectNumber();

	//该接口会有变化
	UINT GetDrawLightObjectNumber();

	//构建我们的视口常量缓冲区视图
	void BuildViewportConstantBufferView();

public:
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void DrawMesh(float DeltaTime);
public:
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	//常量缓冲区
	FConstantBufferViews MeshConstantBufferViews;
	FConstantBufferViews MaterialConstantBufferViews;
	FConstantBufferViews ViewportConstantBufferViews;
	FConstantBufferViews LightConstantBufferViews;
};
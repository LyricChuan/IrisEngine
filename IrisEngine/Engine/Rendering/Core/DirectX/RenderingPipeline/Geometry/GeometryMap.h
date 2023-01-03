#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DirectXDescriptorHeap.h"
#include "../ConstantBuffer/ConstantBufferViews.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"
#include "../DynamicMap/ShadowMap/DynamicShadowMap.h"
#include "../DynamicMap/ShadowMap/DynamicShadowCubeMap.h"

class CMaterial;
struct FRenderingTexture;
class CFogComponent;
struct FGeometry :public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;

	bool IsRenderingDataExistence(CMeshComponent* InKey);

	void BuildMesh(const size_t InMeshHash,CMeshComponent* InMesh, const FMeshRenderingData& MeshData,int InKey);
	void DuplicateMesh(CMeshComponent* InMesh,std::shared_ptr<FRenderingData>& MeshData, int InKey);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData,int InRenderLayerIndex = -1);

	//构建模型
	void Build();

	UINT GetDrawObjectNumber() const;

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

	//真实的数据
	FMeshRenderingData MeshRenderingData;
protected:
	//唯一性的渲染池
	static map<size_t, std::shared_ptr<FRenderingData>> UniqueRenderingDatas;

public:
	//真正的渲染池 里面会有重复的 key (size_t)
	static vector<std::shared_ptr<FRenderingData>> RenderingDatas;
};

//提供渲染内容的接口
struct FGeometryMap :public IDirectXDeviceInterface_Struct
{
	friend class FRenderLayer;
	friend class FDynamicCubeMap;
	friend class FDynamicShadowMap;
	friend class FRenderingPipeline;

	FGeometryMap();
	~FGeometryMap();

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateCalculationsViewport(
		float DeltaTime, 
		const FViewportInfo& ViewportInfo,
		UINT InConstantBufferOffset);

	void UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateLight(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateFog(float DeltaTime, const FViewportInfo& ViewportInfo);

	//收集动态反射模型
	void BuildDynamicReflectionMesh();
	void BuildFog();
	void BuildShadow();
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture();

	//构建模型
	void Build();

	//描述堆
	void BuildDescriptorHeap();

	//构建Mesh常量缓冲区
	void BuildMeshConstantBuffer();

	//构建雾气常量缓冲区
	void BuildFogConstantBuffer();

	//构建Material常量缓冲区
	void BuildMaterialShaderResourceView();

	//构建Material常量缓冲区
	void BuildLightConstantBuffer();

	//该接口会有变化
	UINT GetDrawMeshObjectNumber();

	//该接口会有变化
	UINT GetDrawMaterialObjectNumber();

	//该接口会有变化
	UINT GetDrawLightObjectNumber();
	
	//2D贴图数量
	UINT GetDrawTexture2DResourcesNumber();

	//CubeMap贴图数量
	UINT GetDrawCubeMapResourcesNumber();

	//动态摄像机
	UINT GetDynamicReflectionViewportNum();

	//构建我们的贴图SRV视图
	void BuildTextureConstantBuffer();

	//构建我们的视口常量缓冲区视图
	void BuildViewportConstantBufferView(UINT InViewportOffset = 0);
public:
	UINT GetDynamicReflectionMeshComponentsSize();
	CMeshComponent* GetDynamicReflectionMeshComponents(int Index);

	UINT GetViewportConstantBufferByteSize();

	D3D12_GPU_VIRTUAL_ADDRESS ViewportGPUVirtualAddress();

	bool IsStartUpFog();

public:
	std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string& InKey);
public:
	void DrawShadow(float DeltaTime);
	void DrawLight(float DeltaTime);
	void DrawViewport(float DeltaTime);
	void DrawMesh(float DeltaTime);
	void DrawMaterial(float DeltaTime);
	void Draw2DTexture(float DeltaTime);
	void DrawCubeMapTexture(float DeltaTime);
	void DrawFog(float DeltaTime);
public:
	ID3D12DescriptorHeap* GetHeap()const {return DescriptorHeap.GetHeap();}

protected:
	map<int, FGeometry> Geometrys;
	FDirectXDescriptorHeap DescriptorHeap;

	//常量缓冲区
	FConstantBufferViews MeshConstantBufferViews;
	FConstantBufferViews MaterialConstantBufferViews;
	FConstantBufferViews ViewportConstantBufferViews;
	FConstantBufferViews FogConstantBufferViews;
	FConstantBufferViews LightConstantBufferViews;

	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingTexture2DResources;
	std::shared_ptr<class FRenderingTextureResourcesUpdate> RenderingCubeMapResources;
	std::vector<CMaterial*> Materials;
	std::vector<CMeshComponent*> DynamicReflectionMeshComponents;
	CFogComponent* Fog;

	FDynamicShadowMap DynamicShadowMap;
	FDynamicShadowCubeMap DynamicShadowCubeMap;
};
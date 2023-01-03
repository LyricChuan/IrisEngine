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

	//����ģ��
	void Build();

	UINT GetDrawObjectNumber() const;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
protected:
	//��CPUʹ�õ���Դ
	ComPtr<ID3DBlob> CPUVertexBufferPtr;
	ComPtr<ID3DBlob> CPUIndexBufferPtr;

	//��GPUʹ�õ���Դ
	ComPtr<ID3D12Resource> GPUVertexBufferPtr;
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;

	ComPtr<ID3D12Resource> VertexBufferTmpPtr;
	ComPtr<ID3D12Resource> IndexBufferTmpPtr;

	//��ʵ������
	FMeshRenderingData MeshRenderingData;
protected:
	//Ψһ�Ե���Ⱦ��
	static map<size_t, std::shared_ptr<FRenderingData>> UniqueRenderingDatas;

public:
	//��������Ⱦ�� ��������ظ��� key (size_t)
	static vector<std::shared_ptr<FRenderingData>> RenderingDatas;
};

//�ṩ��Ⱦ���ݵĽӿ�
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

	//�ռ���̬����ģ��
	void BuildDynamicReflectionMesh();
	void BuildFog();
	void BuildShadow();
	void BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData);
	void DuplicateMesh(CMeshComponent* InMesh, std::shared_ptr<FRenderingData>& MeshData);
	bool FindMeshRenderingDataByHash(const size_t& InHash, std::shared_ptr<FRenderingData>& MeshData, int InRenderLayerIndex = -1);

	void LoadTexture();

	//����ģ��
	void Build();

	//������
	void BuildDescriptorHeap();

	//����Mesh����������
	void BuildMeshConstantBuffer();

	//������������������
	void BuildFogConstantBuffer();

	//����Material����������
	void BuildMaterialShaderResourceView();

	//����Material����������
	void BuildLightConstantBuffer();

	//�ýӿڻ��б仯
	UINT GetDrawMeshObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawMaterialObjectNumber();

	//�ýӿڻ��б仯
	UINT GetDrawLightObjectNumber();
	
	//2D��ͼ����
	UINT GetDrawTexture2DResourcesNumber();

	//CubeMap��ͼ����
	UINT GetDrawCubeMapResourcesNumber();

	//��̬�����
	UINT GetDynamicReflectionViewportNum();

	//�������ǵ���ͼSRV��ͼ
	void BuildTextureConstantBuffer();

	//�������ǵ��ӿڳ�����������ͼ
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

	//����������
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
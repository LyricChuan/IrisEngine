#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../RenderingTextureResourcesUpdate.h"
#include "../RenderLayer/RenderLayerManage.h"

UINT MeshObjectCount = 0;

FGeometryMap::FGeometryMap()
{
	Geometrys.insert(pair<int,FGeometry>(0,FGeometry()));

	RenderingTextureResources = std::make_shared<FRenderingTextureResourcesUpdate>();
}

FGeometryMap::~FGeometryMap()
{
	MeshObjectCount = 0;
}

void FGeometryMap::PreDraw(float DeltaTime)
{
	DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
	//��Ⱦ�ӿ�
	DrawViewport(DeltaTime);

	//���Ƶƹ�
	DrawLight(DeltaTime);

	//������ͼ
	DrawTexture(DeltaTime);

	//���Ʋ���
	DrawMaterial(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{

}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	UpdateMaterialShaderResourceView(DeltaTime,ViewportInfo);
	XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
	XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectMatrix);

	//���µƹ�
	FLightConstantBuffer LightConstantBuffer;
	for (size_t i = 0 ;i < GetLightManage()->Lights.size();i++)
	{
		if (CLightComponent* InLightComponent = GetLightManage()->Lights[i])
		{
			fvector_3d LightIntensity = InLightComponent->GetLightIntensity();
			LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
			LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetForwardVector();
			
			LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
			LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();
			
			switch (InLightComponent->GetLightType())
			{
				case ELightType::PointLight:
				case ELightType::SpotLight:
				{
					if (CRangeLightComponent*InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(InLightComponent))
					{
						LightConstantBuffer.SceneLights[i].StartAttenuation = InRangeLightComponent->GetStartAttenuation();
						LightConstantBuffer.SceneLights[i].EndAttenuation = InRangeLightComponent->GetEndAttenuation();
					}
				
					if (InLightComponent->GetLightType() == ELightType::SpotLight)
					{
						if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))
						{
							LightConstantBuffer.SceneLights[i].ConicalInnerCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalInnerCorner());
							LightConstantBuffer.SceneLights[i].ConicalOuterCorner = math_utils::angle_to_radian(InSpotLightComponent->GetConicalOuterCorner());
						}
					}

					break;
				}
			}
		}
	}	
	LightConstantBufferViews.Update(0, &LightConstantBuffer);

	//�����ӿ�
	XMMATRIX ViewProject = XMMatrixMultiply(ViewMatrix, ProjectMatrix);
	FViewportTransformation ViewportTransformation;
	XMStoreFloat4x4(&ViewportTransformation.ViewProjectionMatrix, XMMatrixTranspose(ViewProject));
	
	//�õ��ӿ�λ��
	ViewportTransformation.ViewportPosition = ViewportInfo.ViewPosition;

	ViewportConstantBufferViews.Update(0, &ViewportTransformation);
}

void FGeometryMap::UpdateMaterialShaderResourceView(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FMaterialConstantBuffer MaterialConstantBuffer;
	for (size_t i = 0; i < Materials.size(); i++)
	{
		//�任����
		if (CMaterial* InMaterial = Materials[i])
		{		
			if (InMaterial->IsDirty())
			{
				//BaseColor
				fvector_4d InBaseColor = InMaterial->GetBaseColor();
				MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y, InBaseColor.z, InBaseColor.w);

				fvector_3d InSpecularColor = InMaterial->GetSpecularColor();
				MaterialConstantBuffer.SpecularColor = XMFLOAT3(InSpecularColor.x, InSpecularColor.y, InSpecularColor.z);

				//�ֲڶ�
				MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();

				//��������
				MaterialConstantBuffer.MaterialType = InMaterial->GetMaterialType();

				//�ⲿ��Դ����
				{
					//�����BaseColor
					if (auto BaseColorTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetBaseColorIndexKey()))
					{
						MaterialConstantBuffer.BaseColorIndex = (*BaseColorTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.BaseColorIndex = -1;
					}

					//����
					if (auto NormalTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetNormalIndexKey()))
					{
						MaterialConstantBuffer.NormalIndex = (*NormalTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.NormalIndex = -1;
					}


					//�߹�
					if (auto SpecularTextureResourcesPtr = RenderingTextureResources->FindRenderingTexture(InMaterial->GetSpecularKey()))
					{
						MaterialConstantBuffer.SpecularIndex = (*SpecularTextureResourcesPtr)->RenderingTextureID;
					}
					else
					{
						MaterialConstantBuffer.SpecularIndex = -1;
					}
				}

				//���ʾ���
				XMMATRIX MaterialTransform = XMLoadFloat4x4(&InMaterial->GetMaterialTransform());
				XMStoreFloat4x4(&MaterialConstantBuffer.TransformInformation,
					XMMatrixTranspose(MaterialTransform));

				InMaterial->SetDirty(false);	

				MaterialConstantBufferViews.Update(InMaterial->GetMaterialIndex(), &MaterialConstantBuffer);
			}	
		}
	}
}

void FGeometryMap::BuildMesh(const size_t InMeshHash, CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
	for (auto& Tmp : Geometrys)
	{
		Tmp.second.BuildMesh(InMeshHash, InMesh, MeshData, Tmp.first);
	}
}

void FGeometryMap::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData)
{
	for (auto &Tmp : Geometrys)
	{
		Tmp.second.DuplicateMesh(InMesh, MeshData,Tmp.first);
	}	
}

bool FGeometryMap::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	for (auto &Tmp : Geometrys)
	{
		if (Tmp.second.FindMeshRenderingDataByHash(InHash, MeshData, InRenderLayerIndex))
		{
			return true;
		}
	}

	return false;
}

void FGeometryMap::LoadTexture()
{
	def_c_paths Paths;
	init_def_c_paths(&Paths);

	char RootPath[] = "../IrisEngine/Asset";
	find_files(RootPath, &Paths, true);

	for (int i = 0; i < Paths.index; i++)
	{
		if (find_string(Paths.paths[i],".dds", 0) != -1)
		{
			//��λ��·��
			normalization_path(Paths.paths[i]);

			wchar_t TexturePath[1024] = { 0 };
			char_to_wchar_t(TexturePath, 1024, Paths.paths[i]);

			RenderingTextureResources->LoadTextureResources(TexturePath);
		}	
	}
}

void FGeometryMap::Build()
{
	//����ģ��
	for (auto &Tmp : Geometrys)
	{
		Tmp.second.Build();
	}
}

void FGeometryMap::BuildDescriptorHeap()
{
	//+1�����
	DescriptorHeap.Build(
		GetDrawMeshObjectNumber() +
		1 + //�����
		GetDrawLightObjectNumber() + 
		GetDrawTextureResourcesNumber());//��ͼ
}

void FGeometryMap::BuildMeshConstantBuffer()
{
	//��������������
	MeshConstantBufferViews.CreateConstant(sizeof(FObjectTransformation),GetDrawMeshObjectNumber());

	//Handle
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	MeshConstantBufferViews.BuildConstantBuffer(DesHandle,GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildMaterialShaderResourceView()
{
	//�ռ�����
	//�������Shader-Index
	for(auto& Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto &InData : Tmp->RenderDatas)
		{
			if (auto InMaterials = InData.Mesh->GetMaterials())
			{
				for (size_t j = 0; j < InMaterials->size(); j++)
				{
					//��ShaderIndex����
					(*InMaterials)[j]->SetMaterialIndex(Materials.size());

					Materials.push_back((*InMaterials)[j]);
				}
			}
		}
	}

	//��������������
	MaterialConstantBufferViews.CreateConstant(
		sizeof(FMaterialConstantBuffer),
		GetDrawMaterialObjectNumber(),
		false);
}

void FGeometryMap::BuildLightConstantBuffer()
{
	//��������������
	LightConstantBufferViews.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());

	//Handle
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	LightConstantBufferViews.BuildConstantBuffer(
		DesHandle,
		GetDrawLightObjectNumber(),
		GetDrawMeshObjectNumber());
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
	return Geometrys[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
	return Materials.size();
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
	return 1;
}

UINT FGeometryMap::GetDrawTextureResourcesNumber()
{
	return RenderingTextureResources->Size();
}

void FGeometryMap::BuildTextureConstantBuffer()
{
	RenderingTextureResources->BuildTextureConstantBuffer(
		DescriptorHeap.GetHeap(), 
		GetDrawMeshObjectNumber() + 
		GetDrawLightObjectNumber() +
		1);//�ӿ�
}

void FGeometryMap::BuildViewportConstantBufferView()
{
	//��������������
	ViewportConstantBufferViews.CreateConstant(sizeof(FViewportTransformation), 1);

	//Handle
	CD3DX12_CPU_DESCRIPTOR_HANDLE DesHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

	//��������������
	ViewportConstantBufferViews.BuildConstantBuffer(DesHandle,
		1,
		GetDrawMeshObjectNumber() + 
		GetDrawLightObjectNumber());
}

void FGeometryMap::DrawLight(float DeltaTime)
{
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV���͵�descriptor heap�������CBV
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawMeshObjectNumber(), DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, DesHandle);//3�Ǹ����������2
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV���͵�descriptor heap�������CBV
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawMeshObjectNumber()+
		GetDrawLightObjectNumber(), DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DesHandle);
}

void FGeometryMap::DrawMesh(float DeltaTime)
{
	
}

void FGeometryMap::DrawMaterial(float DeltaTime)
{
	GetGraphicsCommandList()->SetGraphicsRootShaderResourceView(
		4, 
		MaterialConstantBufferViews.GetBuffer()->GetGPUVirtualAddress());
}

void FGeometryMap::DrawTexture(float DeltaTime)
{
	UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto DesHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
	DesHandle.Offset(
		GetDrawMeshObjectNumber() +
		GetDrawLightObjectNumber() + 1, DescriptorOffset);

	GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DesHandle);
}

bool FGeometry::IsRenderingDataExistence(CMeshComponent* InKey)
{
	if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InKey->GetRenderLayerType()))
	{
		for (auto& Tmp : InRenderLayer->RenderDatas)
		{
			if (Tmp.Mesh == InKey)
			{
				return true;
			}
		}
	}

	return false;
}

void FGeometry::BuildMesh(
	const size_t InMeshHash,
	CMeshComponent* InMesh,
	const FMeshRenderingData& MeshData, 
	int InKey)
{
	if (!IsRenderingDataExistence(InMesh))
	{
		//�ҵ���Ӧ�㼶
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			InRenderLayer->RenderDatas.push_back(FRenderingData());
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//����ע��
			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshHash = InMeshHash;
			InRenderingData.GeometryKey = InKey;

		InRenderingData.IndexSize = MeshData.IndexData.size();
		InRenderingData.VertexSize = MeshData.VertexData.size();

		InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();
		InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();

		//��Ч�Ĳ���
		//�����ĺϲ�
		MeshRenderingData.IndexData.insert(
			MeshRenderingData.IndexData.end(),
			MeshData.IndexData.begin(),
			MeshData.IndexData.end());

			//����ĺϲ�
			MeshRenderingData.VertexData.insert(
				MeshRenderingData.VertexData.end(),
				MeshData.VertexData.begin(),
				MeshData.VertexData.end());
		}
	}
}

void FGeometry::DuplicateMesh(CMeshComponent* InMesh, const FRenderingData& MeshData, int InKey)
{
	if (!IsRenderingDataExistence(InMesh))
	{
		if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer((int)InMesh->GetRenderLayerType()))
		{
			InRenderLayer->RenderDatas.push_back(MeshData);
			FRenderingData& InRenderingData = InRenderLayer->RenderDatas[InRenderLayer->RenderDatas.size() - 1];

			//����ע��
			InRenderingData.Mesh = InMesh;
			InRenderingData.MeshObjectIndex = MeshObjectCount++;
			InRenderingData.GeometryKey = InKey;
		}
	}
}

bool FGeometry::FindMeshRenderingDataByHash(const size_t& InHash, FRenderingData& MeshData, int InRenderLayerIndex)
{
	//Ѱ��RenderData
	auto FindMeshRenderingDataByHashSub = [&](std::shared_ptr<FRenderLayer> InRenderLayer)->FRenderingData*
	{
		for (auto& SubTmp : InRenderLayer->RenderDatas)
		{
			if (SubTmp.MeshHash == InHash)
			{
				return &SubTmp;
			}
		}

		return NULL;
	};

	if (InRenderLayerIndex == -1)//������������
	{
		for (auto &Tmp : FRenderLayerManage::RenderLayers)
		{
			if (FRenderingData* InRenderingData = FindMeshRenderingDataByHashSub(Tmp))
			{
				MeshData = *InRenderingData;
				return true;
			}
		}
	}
	//��׼Ѱ��
	else if (std::shared_ptr<FRenderLayer> InRenderLayer = FRenderLayerManage::FindByRenderLayer(InRenderLayerIndex))
	{
		if (FRenderingData *InRenderingData = FindMeshRenderingDataByHashSub(InRenderLayer))
		{
			MeshData = *InRenderingData;
			return true;
		}
	}

	return false;
}

UINT FGeometry::GetDrawObjectNumber() const
{
	return MeshObjectCount;

	int Count = 0;
	for (auto &Tmp : FRenderLayerManage::RenderLayers)
	{
		for (auto &SubTmp : Tmp->RenderDatas)
		{
			Count++;
		}
	}

	return Count;
}

void FGeometry::Build()
{
	//��ȡģ�����ݴ�С
	UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

	ANALYSIS_HRESULT(D3DCreateBlob(
		VertexSizeInBytes,
		&CPUVertexBufferPtr));

	memcpy(CPUVertexBufferPtr->GetBufferPointer(),
		MeshRenderingData.VertexData.data(),VertexSizeInBytes);

	ANALYSIS_HRESULT(D3DCreateBlob(
		IndexSizeInBytes, 
		&CPUIndexBufferPtr));

	memcpy(CPUIndexBufferPtr->GetBufferPointer(),
		MeshRenderingData.IndexData.data(), IndexSizeInBytes);

	ConstructBuffer::FConstructBuffer ConstructBuffer;
	GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		VertexBufferTmpPtr,
		MeshRenderingData.VertexData.data(), VertexSizeInBytes);

	GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(
		IndexBufferTmpPtr,
		MeshRenderingData.IndexData.data(), IndexSizeInBytes);
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
	D3D12_VERTEX_BUFFER_VIEW VBV;
	VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();
	VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
	VBV.StrideInBytes = sizeof(FVertex);

	return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW IBV;
	IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
	IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
	IBV.Format = DXGI_FORMAT_R16_UINT;

	return IBV;
}


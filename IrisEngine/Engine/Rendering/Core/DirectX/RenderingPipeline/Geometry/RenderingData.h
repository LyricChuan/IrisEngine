#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class CMeshComponent;
struct FMeshRenderingData;

//�ṩ��Ⱦ���ݵĽӿ�
struct FRenderingData 
	:public IDirectXDeviceInterface_Struct
	,public std::enable_shared_from_this<FRenderingData>
{
public:
	FRenderingData();

	UINT IndexSize;//Index������
	UINT VertexSize;//Vertex������

	UINT IndexOffsetPosition;//Index��ƫ��λ��
	UINT VertexOffsetPosition;//Index��ƫ��λ��

	UINT VertexTypeSize;
	UINT IndexTypeSize;

public:
	BoundingBox Bounds;//AABB

	size_t MeshHash;
	int GeometryKey;
	int MeshObjectIndex;
public:
	UINT GetVertexSizeInBytes()const { return VertexSize * VertexTypeSize; }
	UINT GetIndexSizeInBytes()const { return IndexSize * IndexTypeSize; }
public:
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;
	XMFLOAT4X4 TextureTransform;

	CMeshComponent* Mesh;//key

	FMeshRenderingData *MeshRenderingData;
};
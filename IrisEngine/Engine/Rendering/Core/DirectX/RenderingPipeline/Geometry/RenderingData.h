#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class CMeshComponent;

//�ṩ��Ⱦ���ݵĽӿ�
struct FRenderingData :public IDirectXDeviceInterface_Struct
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
};
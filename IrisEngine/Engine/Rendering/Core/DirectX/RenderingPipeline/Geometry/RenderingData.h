#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class CMeshComponent;

//提供渲染内容的接口
struct FRenderingData :public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();

	UINT IndexSize;//Index的数量
	UINT VertexSize;//Vertex的数量

	UINT IndexOffsetPosition;//Index的偏移位置
	UINT VertexOffsetPosition;//Index的偏移位置

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
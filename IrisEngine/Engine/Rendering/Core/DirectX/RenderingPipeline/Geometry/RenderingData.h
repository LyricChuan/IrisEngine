#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class GMesh;
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
	UINT GetVertexSizeInBytes()const { return VertexSize * VertexTypeSize; }
	UINT GetIndexSizeInBytes()const { return IndexSize * IndexTypeSize; }
public:
	DXGI_FORMAT IndexFormat;

	XMFLOAT4X4 WorldMatrix;

	GMesh* Mesh;//key
};
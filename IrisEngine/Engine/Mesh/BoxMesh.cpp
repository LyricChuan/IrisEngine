#include "BoxMesh.h"
#include "Core/MeshType.h"

void GBoxMesh::Init()
{
	Super::Init();
}

void GBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	Super::BuildMesh(InRenderingData);


}

void GBoxMesh::Draw(float DeltaTime)
{
	Super::Draw(DeltaTime);
}

void GBoxMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;
	float CDepth = 0.5f * InDepth;

	//构建顶点
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)  ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)  ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, -CDepth),XMFLOAT4(Colors::White)    ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, -CDepth),XMFLOAT4(Colors::White)  ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)   ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(-CWidth, CHeight, CDepth),XMFLOAT4(Colors::White) ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, CHeight, CDepth),XMFLOAT4(Colors::White)   ));
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(CWidth, -CHeight, CDepth),XMFLOAT4(Colors::White)));

	//构建索引
	//前
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
	MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

	//后
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

	//左
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

	//右
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

	//上
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
	MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

	//下
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
	MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);
}


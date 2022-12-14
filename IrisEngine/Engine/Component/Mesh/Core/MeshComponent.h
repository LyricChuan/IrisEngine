#pragma once
#include "../../TransformationComponent.h"
#include "MeshComponentType.h"

class CMaterial;
struct FMeshRenderingData;
class CMeshComponent :public CTransformationComponent
{
	CVARIABLE()
	vector<CMaterial*> Materials;
public:
	CMeshComponent();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	void SetMeshRenderLayerType(EMeshRenderLayerType InRenderLayerType);
public:
	UINT GetMaterialNum()const;

	EMeshRenderLayerType GetRenderLayerType()const { return MeshRenderLayerType; }
	vector<CMaterial*>* GetMaterials() { return &Materials; }
	EMeshRenderLayerType MeshRenderLayerType;

public:
	//��̬����
	bool IsDynamicReflection() const;
};

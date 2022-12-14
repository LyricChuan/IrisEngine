#pragma once
#include "MeshComponent.h"

class CStaticMeshComponent :public CMeshComponent
{
public:
	CStaticMeshComponent();

protected:
	void DrawQuadrilateral(FMeshRenderingData& MeshData,const fvector_4id &InDrawPoint, bool bReversal = false);

	//ר����������ӵ��row��colum���Ե�
	//������������ʱ���Ȼ���һȦrow ����һ���ٻ���һȦrow
	//GroupLayer ������˼�Ļ���һȦrow�������Ƕ���
	//Offset ������˼���ǲ����ڻ������м��ʽ�Ļ���
	fvector_4id GetQuadrilateralDrawPointTypeA(int InRowsand, int InColumns, int GroupLayer, int Offset = 1);
};
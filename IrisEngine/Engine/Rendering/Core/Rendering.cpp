#include "Rendering.h"

vector<FRenderingInterface*> FRenderingInterface::RenderingInterface;

FRenderingInterface::FRenderingInterface()
{
	create_guid(&Guid);

	RenderingInterface.push_back(this);
}

FRenderingInterface::~FRenderingInterface()
{
	for (auto Iter = RenderingInterface.begin(); Iter != RenderingInterface.end(); ++Iter)
	{
		if (*Iter == this)//��Ҫ���ز�����
		{
			RenderingInterface.erase(Iter);
			break;
		}
	}
}

void FRenderingInterface::Draw(float DeltaTime)
{

}


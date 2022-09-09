#pragma once
#include "../../Core/Engine.h"

struct FRenderingInterface
{
	friend class FWindowsEngine;
public:
	FRenderingInterface();
	virtual ~FRenderingInterface();

	void Draw(float DeltaTime);

	bool operator==(const FRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

private:
	static vector<FRenderingInterface*> RenderingInterface;
	simple_c_guid Guid;
};
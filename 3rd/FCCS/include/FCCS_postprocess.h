#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {

	enum class PostProcessEffect
	{
		Copy = 0x1,
	};

	struct PostProcess : public FObject {
	};

	FCCS_API PostProcess* CreatePostProcess(RHIDevice* device, PostProcessEffect effect);
}
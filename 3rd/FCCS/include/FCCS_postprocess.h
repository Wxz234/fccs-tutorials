#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {

	enum class PostProcessEffect
	{
		Copy = 0x1,
	};

	struct PostProcess : public FObject {
		virtual void SetTexture(Texture* texture) = 0;
		virtual void Process(CommandList *list) = 0
	};

	FCCS_API PostProcess* CreatePostProcess(RHIDevice* device, PostProcessEffect effect);
}
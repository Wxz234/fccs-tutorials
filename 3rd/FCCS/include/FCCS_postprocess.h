#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {

	enum class BasicPostProcess
	{
		Copy = 0x1,
	};

	struct PostProcess : public FObject {
		virtual void Process(RHI::CommandList* commandList) = 0;
	};

	FCCS_API PostProcess* CreatePostProcess(RHI::Device* device, BasicPostProcess effect);
}
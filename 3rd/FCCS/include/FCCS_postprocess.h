#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {
	struct PostProcess {
	};

	FCCS_API PostProcess* CreatePostProcess(RHI::Device* pDevice);
	FCCS_API void DestroyPostProcess(PostProcess* pPostProcess);
}
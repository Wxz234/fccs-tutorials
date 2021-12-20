#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {
	struct PostProcess : public FObject {
		virtual void Process(CommandList *pList) = 0;
	};
	FCCS_API PostProcess* CreatePostProcess(RHIDevice* pDevice, DXGI_FORMAT format);
}
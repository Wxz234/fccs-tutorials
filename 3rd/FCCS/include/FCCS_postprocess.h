#pragma once
#include "FCCS_core.h"
#include "FCCS_rhi.h"
namespace FCCS {
	enum class Effect : uint32 {
		Copy
	};
	struct PostProcess : public FObject {
		virtual void SetTexture(Texture* pTexture) = 0;
		virtual void Process(CommandList* pList) = 0;
	};

	PostProcess* CreatePostProcess(Device* pDevice, DXGI_FORMAT rtFormat, DXGI_FORMAT dsFormat, Effect effect);
}
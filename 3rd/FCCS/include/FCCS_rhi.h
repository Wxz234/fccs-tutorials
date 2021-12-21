#pragma once
#include "FCCS_core.h"
#include "FCCS_window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {
	struct Texture : public FRHIObejct {

	};

	struct CommandQueue : public FRHIObejct {

	};

	struct Device : public FRHIObejct {
		virtual CommandQueue* GetRenderCommandQueue() const noexcept = 0;
	};

	struct SwapChain : public FRHIObejct {
		virtual void Present(uint32 syncInterval) = 0;
	};

	FCCS_API Device* CreateDevice(uint32 adapter);
	FCCS_API SwapChain* CreateSwapChain(Device* pDevice, Window* pWindow);
}
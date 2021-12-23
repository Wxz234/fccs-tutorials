#pragma once
#include "FCCS_core.h"
#include "FCCS_window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {

	struct GpuResource : public FRHIObejct {

	};

	struct Texture : public GpuResource {
		
	};

	struct CommandAllocator : public FRHIObejct {
	
	};

	struct CommandList : public FRHIObejct {
		virtual void ResourceBarrier(GpuResource* pRes, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) = 0;
		virtual void Close() = 0;
	};

	struct CommandQueue : public FRHIObejct {
		virtual void Execute(CommandList* pList) = 0;
		virtual void WaitIdle() = 0;
	};

	struct Device : public FRHIObejct {
		virtual CommandQueue* GetRenderCommandQueue() const noexcept = 0;
	};

	struct SwapChain : public FRHIObejct {
		virtual void Present(uint32 syncInterval) = 0;
		virtual DXGI_FORMAT GetRenderTargetFormat() const noexcept = 0;
		virtual DXGI_FORMAT GetDepthStencilFormat() const noexcept = 0;
	};

	FCCS_API Device* CreateDevice(uint32 adapter);
	FCCS_API SwapChain* CreateSwapChain(Device* pDevice, Window* pWindow);
}
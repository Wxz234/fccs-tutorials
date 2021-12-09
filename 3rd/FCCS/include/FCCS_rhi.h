#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {
	struct CommandQueue : public FRHIObejct {

	};

	struct RHIDevice : public FRHIObejct {
		virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
	};
	FCCS_API RHIDevice* CreateDevice(uint32 adapter);

	struct FCCS_SWAP_CHAIN_DESC {
		uint32 BufferCount;
		uint32 Width;
		uint32 Height;
		DXGI_FORMAT Format;
		DXGI_FORMAT DepthStencilFormat;
		HWND Hwnd;
	};

	struct SwapChain : public FRHIObejct {
		virtual void Present(uint32 sync) = 0;
	};
	FCCS_API SwapChain* CreateSwapChain(CommandQueue* queue, const FCCS_SWAP_CHAIN_DESC* desc);
}
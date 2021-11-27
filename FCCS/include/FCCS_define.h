#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {
	struct FCCS_SWAP_CHAIN_DESC {
		uint32 BufferCount;
		uint32 Width;
		uint32 Height;
		DXGI_FORMAT Format;
		DXGI_FORMAT DepthStencilFormat;
		HWND Hwnd;
	};

}

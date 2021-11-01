#pragma once
#include "FCCS_core.h"
#include "FCCS_resource.h"
#include "FCCS_window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {
	namespace Graphics {

		struct FCCS_NOVTABLE IPipelineState : public IResource {
		public:
		};

		struct FCCS_NOVTABLE ICommandList : public IResource {
		public:
			virtual void Reset(IPipelineState *state) = 0;
			virtual void Close() = 0;
		};

		struct FCCS_NOVTABLE ICommandQueue : public IResource {
		public:
			virtual void Execute(ICommandList* list) = 0;
		};

		struct FCCS_NOVTABLE IDevice : public IResource {
		public:
			
		};

		FCCS_API IDevice* CreateDevice();

		struct FCCS_SWAP_CHAIN_DESC {
			uint32 BufferCount;
			uint32 Width;
			uint32 Height;
			DXGI_FORMAT Format;
		};

		struct FCCS_NOVTABLE ISwapChain : public IResource {
			virtual void Present() = 0;
		};
		FCCS_API ISwapChain* CreateSwapChain(ICommandQueue* queue, IWindow* window, const FCCS_SWAP_CHAIN_DESC* desc);
	}
}
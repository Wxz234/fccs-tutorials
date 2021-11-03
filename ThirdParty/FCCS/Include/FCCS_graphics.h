#pragma once
#include "FCCS_core.h"
#include "FCCS_resource.h"
#include "FCCS_window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {
	namespace Graphics {

		struct FCCS_NOVTABLE IGpuResource : public IResource {
			virtual D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress() const = 0;
			virtual void* Map() = 0;
			virtual void Unmap() = 0;
			virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
		};

		struct FCCS_NOVTABLE IPipelineState : public IResource {
		};

		struct FCCS_NOVTABLE ICommandList : public IResource {
			virtual void Reset(IPipelineState *state) = 0;
			virtual void Close() = 0;

			virtual void ResourceBarrier(IGpuResource* resource, D3D12_RESOURCE_STATES brfore, D3D12_RESOURCE_STATES after) = 0;
		};

		struct FCCS_NOVTABLE ICommandQueue : public IResource {
			virtual void Execute(ICommandList* list) = 0;
		};

		struct FCCS_NOVTABLE IDevice : public IResource {
			virtual bool CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type, ICommandQueue** ppCommandQueue) = 0;
			virtual bool CreateCommandList(D3D12_COMMAND_LIST_TYPE type, ICommandList** ppCommandList) = 0;
			virtual bool CreateBuffer(D3D12_HEAP_TYPE type, uint64 size, D3D12_RESOURCE_STATES state, IGpuResource** ppBuffer) = 0;
		};

		struct FCCS_SWAP_CHAIN_DESC {
			uint32 BufferCount;
			uint32 Width;
			uint32 Height;
			DXGI_FORMAT Format;
		};

		struct FCCS_NOVTABLE ISwapChain : public IResource {
			virtual void Present() = 0;
			virtual uint32 GetCurrentBackBufferIndex() const = 0;
			virtual bool GetBuffer(uint32 n, IGpuResource** ppBuffer) = 0;
		};

		FCCS_API bool CreateDevice(IDevice** ppDevice);
		FCCS_API bool CreateSwapChain(ICommandQueue* queue, IWindow* window, const FCCS_SWAP_CHAIN_DESC* desc, ISwapChain** ppSwapChain);
	}
}
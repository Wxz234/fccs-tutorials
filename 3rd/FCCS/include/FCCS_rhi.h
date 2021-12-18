#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {

	struct Buffer : public FRHIObejct {
		virtual void Update(void* pData, uint32 size) = 0;
		virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
	};

	struct Texture : public FRHIObejct {
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() = 0;
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetUnorderedAccessView() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() = 0;
	};

	struct CommandList : public FRHIObejct {
		virtual void Close() = 0;
	};
	struct CommandQueue : public FRHIObejct {
		virtual void WaitIdle() = 0;
	};

	struct RHIDevice : public FRHIObejct {
		virtual Buffer* CreateBuffer(uint32 size) = 0;
		virtual Texture* CreateDDSTextureFromFile(const wchar_t* pFileName) = 0;
		virtual Texture* CreateTexture(const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr) = 0;
		virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual CommandList* CreateCommandList(D3D12_COMMAND_LIST_TYPE type) = 0;
	};

	struct SwapChain : public FRHIObejct {
		virtual void Present(uint32 syncInterval) = 0;
		virtual uint32 GetCurrentBackBufferIndex() const = 0;
	};

	FCCS_API RHIDevice* CreateDevice(uint32 adapter);
}
#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {

	struct RootSignature : public FRHIObejct {};
	struct PSO : public FRHIObejct {};

	struct Buffer : public FRHIObejct {
		virtual void Update(void* pData, uint32 size) = 0;
		virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
	};


	struct CommandList : public FRHIObejct {
		virtual void Reset(PSO* pInitialState) = 0;
		virtual void Close() = 0;
	};
	struct CommandQueue : public FRHIObejct {
		virtual void WaitIdle() = 0;
	};

	struct Texture : public FRHIObejct {
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() = 0;
	};

	struct RenderTarget : public FRHIObejct {
		virtual void Begin(CommandList *pList) = 0;
		virtual void End(CommandList* pList) = 0;
	};

	struct RHIDevice : public FRHIObejct {
		virtual Buffer* CreateBuffer(uint32 size) = 0;
		virtual Texture* CreateDDSTextureFromFile(const wchar_t* pFileName) = 0;
		virtual Texture* CreateTexture(const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState) = 0;
		virtual RenderTarget* CreateRenderTarget(const D3D12_RESOURCE_DESC* pDesc, D3D12_RESOURCE_STATES InitialResourceState, const D3D12_CLEAR_VALUE* pOptimizedClearValue) = 0;
		virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual CommandList* CreateCommandList(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC* pDesc) = 0;
		virtual PSO* CreateGraphicsPSO(const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc) = 0;
	};

	struct FCCS_SWAP_CHAIN_DESC {
		uint32 BufferCount;
		uint32 Width;
		uint32 Height;
		DXGI_FORMAT Format;
		HWND Hwnd;
	};

	struct SwapChain : public FRHIObejct {
		virtual void Present(uint32 syncInterval) = 0;
		virtual uint32 GetCurrentBackBufferIndex() const = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32 n) = 0;
	};

	struct Blob : public FRHIObejct {
	};

	FCCS_API RHIDevice* CreateDevice(uint32 adapter);
	FCCS_API SwapChain* CreateSwapChain(CommandQueue* pQueue, const FCCS_SWAP_CHAIN_DESC* pDesc);
	FCCS_API Blob* CompileShaderFromFile(const wchar_t* pFileName, const char* pEntrypoint, const char* pTarget);
}
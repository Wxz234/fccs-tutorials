#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {

	struct RootSignature : public FRHIObejct {};
	struct PSO : public FRHIObejct {};

	struct Buffer : public FRHIObejct {
		virtual void Update(void* ptr, uint32 size) = 0;
		virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
		virtual D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(uint32 size, DXGI_FORMAT format) = 0;
	};

	struct Texture : public FRHIObejct {
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() = 0;
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetUnorderedAccessView() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() = 0;
	};

	struct CommandList : public FRHIObejct {
		virtual void Reset(PSO* pso) = 0;
		virtual void Close() = 0;
	};
	struct CommandQueue : public FRHIObejct {
		virtual void WaitIdle() = 0;
	};

	struct RHIDevice : public FRHIObejct {
		virtual Buffer* CreateBuffer(uint32 size) = 0;
		virtual Texture* CreateDDSTextureFromFile(const wchar_t* szFileName) = 0;
		virtual Texture* CreateRenderTargetTexture(DXGI_FORMAT format, uint32 width, uint32 height,const D3D12_CLEAR_VALUE *value) = 0;
		virtual Texture* CreateDepthStencilTexture(DXGI_FORMAT format, uint32 width, uint32 height,const D3D12_CLEAR_VALUE *value) = 0;
		virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual CommandList* CreateCommandList(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual RootSignature* CreateRootSignature(const D3D12_ROOT_SIGNATURE_DESC* desc) = 0;
		virtual PSO* CreateGraphicsPSO(const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc) = 0;
	};

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
		virtual uint32 GetCurrentBackBufferIndex() const = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32 n) = 0;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() = 0;
		virtual Texture* GetTexture(uint32 n) = 0;
	};

	struct Blob : public FRHIObejct {
	};

	FCCS_API RHIDevice* CreateDevice(uint32 adapter);
	FCCS_API SwapChain* CreateSwapChain(CommandQueue* queue, const FCCS_SWAP_CHAIN_DESC* desc);
	FCCS_API Blob* CompileShaderFromFile(const wchar_t* pFileName, const char* pEntrypoint, const char* pTarget);
}
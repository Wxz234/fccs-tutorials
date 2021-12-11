#pragma once
#include "FCCS_core.h"
#include <d3d12.h>
#include <dxgi1_6.h>
namespace FCCS {

	struct Buffer : public FRHIObejct {
		virtual void Update(void* ptr, uint32 size) = 0;
		virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
		virtual D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(uint32 size, DXGI_FORMAT format) = 0;
	};

	struct Texture : public FRHIObejct {
		virtual D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() = 0;
	};

	struct CommandList : public FRHIObejct {
		virtual void Reset(ID3D12PipelineState* pso) = 0;
		virtual void Close() = 0;
	};
	struct CommandQueue : public FRHIObejct {
		virtual void WaitIdle() = 0;
	};

	struct RHIDevice : public FRHIObejct {
		virtual Buffer* CreateBuffer(uint32 size) = 0;
		virtual Texture* CreateDDSTextureFromFile(const wchar_t* szFileName) = 0;
		virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
		virtual CommandList* CreateCommandList(D3D12_COMMAND_LIST_TYPE type) = 0;
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
	};

	struct Blob : public FRHIObejct {
	};

	FCCS_API RHIDevice* CreateDevice(uint32 adapter);
	FCCS_API SwapChain* CreateSwapChain(CommandQueue* queue, const FCCS_SWAP_CHAIN_DESC* desc);
	FCCS_API Blob* CompileShaderFromFile(const wchar_t* pFileName, const char* pEntrypoint, const char* pTarget);
}
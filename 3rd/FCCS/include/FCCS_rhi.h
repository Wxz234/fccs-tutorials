#pragma once
#include "FCCS_core.h"
#include "FCCS_define.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
namespace FCCS {
	namespace RHI {

        struct RootSignature : public FObject {
			virtual ID3D12RootSignature* GetRootSignaturePtr() const = 0;
		};

		struct GPUResource : public FObject {
			virtual ID3D12Resource* GetResourcePtr() const = 0;
		};

		struct PSO :public FObject {
			virtual ID3D12PipelineState* GetPipelineStatePtr() const = 0;
		};

		struct Texture : public GPUResource {
			virtual uint32 GetWidth() const = 0;
			virtual uint32 GetHeight() const = 0;
			virtual DXGI_FORMAT GetFormat() const = 0;
			virtual D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceView() const = 0;
		};

		struct Buffer : public GPUResource {
			virtual void Update(void* ptr, uint32 size) = 0;
			virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
			virtual D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(uint32 size, DXGI_FORMAT format) = 0;
		};

		struct StaticBuffer : public GPUResource {
			virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
			virtual D3D12_INDEX_BUFFER_VIEW GetIndexBufferView(uint32 size, DXGI_FORMAT format) = 0;
		};

		struct CommandList : public FObject {
			virtual void Reset(PSO *pso) = 0;
			virtual ID3D12GraphicsCommandList* GetCommandListPtr() const = 0;
		};

		struct CommandQueue : public FObject {
			virtual void WaitIdle() = 0;
			virtual ID3D12CommandQueue* GetCommandQueuePtr() const = 0;
		};

		struct Device : public FObject {
			virtual Buffer* CreateBuffer(uint32 size) = 0;
			virtual StaticBuffer* CreateStaticBuffer(void* ptr, uint32 size) = 0;
			virtual CommandQueue* CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type) = 0;
			virtual CommandList* CreateCommandList(D3D12_COMMAND_LIST_TYPE type) = 0;
			virtual Texture* CreateDDSTextureFromFile(const wchar_t* szFileName) = 0;
			virtual RootSignature* CreateRootSignature(uint32 numParameters, const D3D12_ROOT_PARAMETER* _pParameters, uint32 numStaticSamplers = 0, const D3D12_STATIC_SAMPLER_DESC* _pStaticSamplers = nullptr, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE) = 0;
			virtual PSO* CreateGraphicsPSO(const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc) = 0;
			virtual ID3D12Device* GetDevicePtr() const = 0;
			virtual CommandQueue* GetDefaultCommandQueue() const = 0;
		};

		struct SwapChain : public FObject {
			virtual void Present() = 0;
			virtual uint32 GetBackBufferIndex()const = 0;
			virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32 n) = 0;
			virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() = 0;
			virtual ID3D12Resource* GetResource(uint32 n) const = 0;
			virtual IDXGISwapChain* GetSwapChainPtr() const = 0;
		};

		struct Blob : public FObject {
			virtual ID3DBlob* GetBlobPtr() const = 0;
		};

		FCCS_API bool CreateDeviceAndSwapChain(const FCCS_SWAP_CHAIN_DESC* desc, Device **ppDevice, SwapChain **ppSwapchain);
		FCCS_API Blob* CompileShaderFromFile(const wchar_t* pFileName, const char* pEntrypoint, const char* pTarget);
	}
}
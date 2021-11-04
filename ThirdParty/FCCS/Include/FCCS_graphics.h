#pragma once
#include "FCCS_core.h"
#include "FCCS_resource.h"
#include "FCCS_window.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
namespace FCCS {


	struct FCCS_SHADER_BYTECODE : public D3D12_SHADER_BYTECODE
	{
		FCCS_SHADER_BYTECODE() = default;
		explicit FCCS_SHADER_BYTECODE(const D3D12_SHADER_BYTECODE& o) noexcept :
			D3D12_SHADER_BYTECODE(o)
		{}
		FCCS_SHADER_BYTECODE(
			_In_ ID3DBlob* pShaderBlob) noexcept
		{
			pShaderBytecode = pShaderBlob->GetBufferPointer();
			BytecodeLength = pShaderBlob->GetBufferSize();
		}
		FCCS_SHADER_BYTECODE(
			const void* _pShaderBytecode,
			SIZE_T bytecodeLength) noexcept
		{
			pShaderBytecode = _pShaderBytecode;
			BytecodeLength = bytecodeLength;
		}
	};

	namespace Graphics {

		struct FCCS_GRAPHICS_PIPELINE_STATE_DESC  {
			FCCS_GRAPHICS_PIPELINE_STATE_DESC() {
				ZeroMemory(&m_PSODesc, sizeof(m_PSODesc));
				m_PSODesc.NodeMask = 1;
				m_PSODesc.SampleMask = 0xFFFFFFFFu;
				m_PSODesc.SampleDesc.Count = 1;
				m_PSODesc.InputLayout.NumElements = 0;
			}
			D3D12_GRAPHICS_PIPELINE_STATE_DESC GetPSO() const {
				return m_PSODesc;
			}

			void SetRootSignature(ID3D12RootSignature* rootSignature) {
				_root = rootSignature;
				m_PSODesc.pRootSignature = _root.Get();
			}
			void SetBlendState(const D3D12_BLEND_DESC& BlendDesc) {
				m_PSODesc.BlendState = BlendDesc;
			}
			void SetRasterizerState(const D3D12_RASTERIZER_DESC& RasterizerDesc) {
				m_PSODesc.RasterizerState = RasterizerDesc;
			}
			void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& DepthStencilDesc) {
				m_PSODesc.DepthStencilState = DepthStencilDesc;
			}
			void SetSampleMask(UINT SampleMask) {
				m_PSODesc.SampleMask = SampleMask;
			}
			void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE TopologyType) {
				m_PSODesc.PrimitiveTopologyType = TopologyType;
			}
			void SetDepthTargetFormat(DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0) {
				SetRenderTargetFormats(0, nullptr, DSVFormat, MsaaCount, MsaaQuality);
			}
			void SetRenderTargetFormat(DXGI_FORMAT RTVFormat, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0) {
				SetRenderTargetFormats(1, &RTVFormat, DSVFormat, MsaaCount, MsaaQuality);
			}
			void SetRenderTargetFormats(UINT NumRTVs, const DXGI_FORMAT* RTVFormats, DXGI_FORMAT DSVFormat, UINT MsaaCount = 1, UINT MsaaQuality = 0) {
				//ASSERT(NumRTVs == 0 || RTVFormats != nullptr, "Null format array conflicts with non-zero length");
				for (UINT i = 0; i < NumRTVs; ++i)
				{
					//ASSERT(RTVFormats[i] != DXGI_FORMAT_UNKNOWN);
					m_PSODesc.RTVFormats[i] = RTVFormats[i];
				}
				for (UINT i = NumRTVs; i < m_PSODesc.NumRenderTargets; ++i)
					m_PSODesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
				m_PSODesc.NumRenderTargets = NumRTVs;
				m_PSODesc.DSVFormat = DSVFormat;
				m_PSODesc.SampleDesc.Count = MsaaCount;
				m_PSODesc.SampleDesc.Quality = MsaaQuality;
			}
			void SetInputLayout(UINT NumElements, const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs) {
				if (NumElements > 0) {
					m_PSODesc.InputLayout.NumElements = NumElements;
					auto* ptr = new D3D12_INPUT_ELEMENT_DESC[NumElements];
					m_InputLayouts.reset(ptr);
					MemCopyU64(ptr, pInputElementDescs, NumElements * sizeof(D3D12_INPUT_ELEMENT_DESC));
				}
			}
			void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBProps) {
				m_PSODesc.IBStripCutValue = IBProps;
			}

			// These const_casts shouldn't be necessary, but we need to fix the API to accept "const void* pShaderBytecode"
			void SetVertexShader(const void* Binary, size_t Size) { m_PSODesc.VS = FCCS_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
			void SetPixelShader(const void* Binary, size_t Size) { m_PSODesc.PS = FCCS_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
			void SetGeometryShader(const void* Binary, size_t Size) { m_PSODesc.GS = FCCS_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
			void SetHullShader(const void* Binary, size_t Size) { m_PSODesc.HS = FCCS_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }
			void SetDomainShader(const void* Binary, size_t Size) { m_PSODesc.DS = FCCS_SHADER_BYTECODE(const_cast<void*>(Binary), Size); }

			void SetVertexShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.VS = Binary; }
			void SetPixelShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.PS = Binary; }
			void SetGeometryShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.GS = Binary; }
			void SetHullShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.HS = Binary; }
			void SetDomainShader(const D3D12_SHADER_BYTECODE& Binary) { m_PSODesc.DS = Binary; }
		private:
			D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PSODesc;
			std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC[]> m_InputLayouts;
			RefCountPtr<ID3D12RootSignature> _root;
		};

		struct FCCS_NOVTABLE IGpuResource : public IResource {
			virtual D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress() const = 0;
			virtual void* Map() = 0;
			virtual void Unmap() = 0;
			virtual D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(uint32 stride, uint32 size) = 0;
		};

		struct FCCS_NOVTABLE IPipelineState : public IResource {};

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
			virtual bool CreateGraphicsPipelineState(const FCCS_GRAPHICS_PIPELINE_STATE_DESC* desc, IPipelineState** ppPipelineState) = 0;
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
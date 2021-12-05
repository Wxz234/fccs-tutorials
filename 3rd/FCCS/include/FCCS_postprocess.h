#pragma once
#include "FCCS_core.h"
namespace FCCS {

	enum class BasicPostProcess
	{
		Copy = 0x1,
	};

	struct PostProcess {
		virtual void Release() = 0;
		virtual void SetSourceTexture(D3D12_GPU_DESCRIPTOR_HANDLE srvDescriptor, ID3D12Resource* resource) = 0;
		virtual void Process(ID3D12GraphicsCommandList* commandList) = 0;
	};

	FCCS_API PostProcess* CreatePostProcess(ID3D12Device* device);
}
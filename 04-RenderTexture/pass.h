#pragma once
#include <fccs/fccs.h>
#include <d3dx12.h>

class RenderTexturePass {
public:
	RenderTexturePass(fccs::rhi::IDevice* pDevice, uint32_t width, uint32_t height) {
		list = pDevice->CreateCommandList(fccs::rhi::CommandQueueType::Graphics);
		fccs::rhi::TextureDesc desc = {};
		desc.width = width;
		desc.height = height;
		desc.format = fccs::rhi::Format::RGBA8_UNORM;
		desc.isRenderTarget = true;
		texture = pDevice->CreateTexture(desc);
	}
	~RenderTexturePass() { fccs::DestroyResource(list); fccs::DestroyResource(texture);}
	void Execute() {
		list->Open();
		list->ChangeTextureState(texture, fccs::rhi::ResourceStates::RenderTarget);
		list->Close();
	}
	fccs::rhi::ICommandList* list;
	fccs::rhi::ITexture* texture;
};

class RenderSwapChainPass {
public:
	RenderSwapChainPass(fccs::rhi::IDevice* pDevice, fccs::rhi::ISwapChain *pSwapchain) {
		list = pDevice->CreateCommandList(fccs::rhi::CommandQueueType::Graphics);
		swapchain = pSwapchain;
	}
	~RenderSwapChainPass() { fccs::DestroyResource(list); }
	void Execute() {
		list->Open();
		auto index = swapchain->GetCurrentBackBufferIndex();
		list->ChangeTextureState(swapchain->GetTexture(index), fccs::rhi::ResourceStates::RenderTarget);
		
		list->Close();
	}
	fccs::rhi::ICommandList* list;
	fccs::rhi::ISwapChain* swapchain;
};


#pragma once
#include <fccs/fccs.h>
#include <d3dx12.h>
class RenderTexturePass {
public:
	RenderTexturePass(fccs::rhi::IDevice *pDevice){
		list = pDevice->CreateCommandList(fccs::rhi::CommandQueueType::Graphics);
	}
	~RenderTexturePass() { fccs::DestroyResource(list); }
	void Execute() {
		list->Open();
		list->Close();
	}
	fccs::rhi::ICommandList* list;
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
		list->Close();
	}
	fccs::rhi::ICommandList* list;
	fccs::rhi::ISwapChain* swapchain;
};


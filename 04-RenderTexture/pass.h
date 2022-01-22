#pragma once
#include <fccs/fccs.h>
#include <thread>

class RenderTexturePass {
public:
	RenderTexturePass(fccs::rhi::IDevice *pDevice){
		list = pDevice->CreateCommandList(fccs::rhi::CommandQueueType::Graphics);
	}
	~RenderTexturePass() { fccs::DestroyResource(list); }
	void Execute() {
		std::thread t(
			[this]() {
				list->Open();
				list->Close();
			}
		);
		t.join();
	}

	fccs::rhi::ICommandList* list;
};

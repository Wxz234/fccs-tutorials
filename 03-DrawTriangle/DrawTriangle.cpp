#include <FCCS.h>
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include <d3dx12.h>
#include <Windows.h>
#include "Vertex.h"
using namespace FCCS;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr uint32 frameCount = 3;
	RefCountPtr<IWindow> window;
	CreateWindowExW(L"fccs", 800, 600, &window);
	RefCountPtr<Graphics::IDevice> device;
	Graphics::CreateDevice(&device);
	RefCountPtr<Graphics::ICommandQueue> comandqueue;
	device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,&comandqueue);
	Graphics::FCCS_SWAP_CHAIN_DESC desc{};
	desc.BufferCount = frameCount;
	desc.Width = 800;
	desc.Height = 600;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RefCountPtr<Graphics::ISwapChain> swapchain;
	Graphics::CreateSwapChain(comandqueue.Get(), window.Get(), &desc, &swapchain);
	
	RefCountPtr<Graphics::ICommandList> commandlists[frameCount];
	RefCountPtr<Graphics::IGpuResource> swapchainbuffer[frameCount];
	for (uint32 i = 0; i < frameCount; ++i) {
		device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &commandlists[i]);
		swapchain->GetBuffer(i, &swapchainbuffer[i]);
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	while (window->IsActive()) {
		const uint32 frameIndex = swapchain->GetCurrentBackBufferIndex();
		commandlists[frameIndex]->Reset(nullptr);
		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);




		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandlists[frameIndex]->Close();
		comandqueue->Execute(commandlists[frameIndex].Get());
		swapchain->Present();
	}
	return 0;
}
#include <Windows.h>
#include <FCCS.h>
#include <vector>
#include <d3dcommon.h>
#define D3DX12_NO_STATE_OBJECT_HELPERS
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include "d3dx12.h"
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr unsigned width = 800, height = 600;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);
	auto buffercount = swapchain->GetBufferCount();

	auto commandqueue = device->GetRenderCommandQueue();
	auto commandlist = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	std::vector<FCCS::CommandAllocator*> commandallocator;
	for (FCCS::uint32 i = 0;i < buffercount; ++i) {
		commandallocator.emplace_back(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT));
	}

	while (window->IsRun()) {
		swapchain->Present(1);
	}

	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	FCCS::DestroyFObject(commandlist);
	for (auto alloc : commandallocator) {
		FCCS::DestroyFObject(alloc);
	}
	return 0;
}
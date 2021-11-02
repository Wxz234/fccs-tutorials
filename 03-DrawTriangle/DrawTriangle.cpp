#include <Windows.h>
#include <FCCS.h>
using namespace FCCS;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	RefCountPtr<IWindow> window;
	CreateWindowExW(L"fccs", 800, 600, &window);
	RefCountPtr<Graphics::IDevice> device;
	Graphics::CreateDevice(&device);
	RefCountPtr<Graphics::ICommandQueue> comandqueue;
	device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,&comandqueue);
	Graphics::FCCS_SWAP_CHAIN_DESC desc{};
	desc.BufferCount = 3;
	desc.Width = 800;
	desc.Height = 600;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RefCountPtr<Graphics::ISwapChain> swapchain;
	Graphics::CreateSwapChain(comandqueue.Get(), window.Get(), &desc, &swapchain);
	
	window->ShowWindow();
	while (window->IsActive()) {
		swapchain->Present();
	}
	return 0;
}
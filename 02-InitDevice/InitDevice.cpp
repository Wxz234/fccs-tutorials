#include <Windows.h>
#include <FCCS.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", 800, 600);
	auto device = FCCS::CreateDevice(0);
	auto queue = device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	auto swapchain = FCCS::CreateSwapChain(queue, window->GetHWND(), window->GetWidth(), window->GetHeight());
	while (window->IsRun()) {
		swapchain->Present(1);
	}
	FCCS::DestroyObject(window);
	FCCS::DestroyObject(device);
	FCCS::DestroyObject(queue);
	FCCS::DestroyObject(swapchain);
	return 0;
}
#include <Windows.h>
#include <FCCS.h>
#include <wrl.h>
#pragma comment(lib,"runtimeobject.lib")
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		return 1;
	constexpr unsigned width = 800, height = 600;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);
	while (window->IsRun()) {
		swapchain->Present(1);
	}
	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	return 0;
}
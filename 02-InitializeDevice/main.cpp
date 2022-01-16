#include <Windows.h>
#include <fccs/fccs.h>

class Callback : public fccs::window::IWindowCallback {
public:
	Callback(HWND hwnd):hwnd(hwnd) {}
	void Initialize() {
		device = fccs::rhi::CreateDeivce();
		queue = device->CreateCommandQueue(fccs::rhi::CommandQueueType::Graphics);
		fccs::window::SwapChainDesc swapchainDesc = {};
		swapchainDesc.width = 800;
		swapchainDesc.height = 600;
		swapchainDesc.hwnd = hwnd;
		swapchain = fccs::window::CreateSwapChain(swapchainDesc, queue.get());
	}
	void Update() {
		swapchain->Present(1);
	}
	void Release() {}
private:
	HWND hwnd;
	fccs::rhi::DeviceHandle device;
	fccs::rhi::CommandQueueHandle queue;
	fccs::window::SwapChainHandle swapchain;
};


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	auto window = fccs::window::CreateFCCSWindow();
	window->OpenWindow();
	Callback callback(window->GetHWND());
	return window->Run(&callback);
}

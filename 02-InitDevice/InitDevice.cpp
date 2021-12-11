#include <Windows.h>
#include <FCCS.h>
constexpr unsigned width = 800, height = 600;
constexpr unsigned bufferCount = 3;
class Context : public FCCS::WindowContext {
public:
	Context(HWND hwnd) : hwnd(hwnd) {}

	void Initialize() {

		FCCS::FCCS_SWAP_CHAIN_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.Hwnd = hwnd;
		desc.BufferCount = bufferCount;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		device = FCCS::CreateDevice(0);
		queue = device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		swapchain = FCCS::CreateSwapChain(queue, &desc);
	}
	void Update() {
		swapchain->Present(1);
	}
	void Release() {
		FCCS::DestroyObject(device);
		FCCS::DestroyObject(queue);
		FCCS::DestroyObject(swapchain);
	}

	HWND hwnd;

	FCCS::RHIDevice* device = nullptr;
	FCCS::CommandQueue* queue = nullptr;
	FCCS::SwapChain* swapchain = nullptr;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", width, height);
	Context context(window->GetHWND());
	return window->Run(&context);
}
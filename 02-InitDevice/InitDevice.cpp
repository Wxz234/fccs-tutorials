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
		FCCS::RHI::CreateDeviceAndSwapChain(&desc, &device, &swapchain);
	}
	void Update() {
		swapchain->Present();
	}
	void Release() {
		FCCS::RHI::DestroyRHIObject(device);
		FCCS::RHI::DestroyRHIObject(swapchain);
	}

	HWND hwnd;

	FCCS::RHI::Device* device = nullptr;
	FCCS::RHI::SwapChain* swapchain = nullptr;
};
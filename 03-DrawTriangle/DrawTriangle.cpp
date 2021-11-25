#include <Windows.h>
#include <FCCS.h>
class Context : public FCCS::WindowContext {
public:
	Context(const FCCS::FCCS_SWAP_CHAIN_DESC* pDesc) {
		desc = *pDesc;
		device = nullptr;
		swapchain = nullptr;
	}

	void Initialize() {
		FCCS::RHI::CreateDeviceAndSwapChain(&desc, &device, &swapchain);
	}
	void Update() {
		swapchain->Present();
	}
	void Release() {
		FCCS::RHI::DestroyRHIObject(device);
		FCCS::RHI::DestroyRHIObject(swapchain);
	}

	FCCS::RHI::Device* device;
	FCCS::RHI::SwapChain* swapchain;
	FCCS::FCCS_SWAP_CHAIN_DESC desc;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	unsigned width = 800, height = 600;
	constexpr unsigned bufferCount = 3;
	auto window = FCCS::CreateWindowExW(L"fccs", width, height);
	FCCS::FCCS_SWAP_CHAIN_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.Hwnd = window->GetHWND();
	desc.BufferCount = bufferCount;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Context context(&desc);
	return window->Run(&context);
}
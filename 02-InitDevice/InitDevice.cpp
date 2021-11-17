#include <Windows.h>
#include <FCCS.h>
class Context : public FCCS::WindowContext {
public:
	Context(const FCCS::RHI::FCCS_SWAP_CHAIN_DESC* pDesc) {
		desc = *pDesc;
		queue = nullptr;
		device = nullptr;
		swapchain = nullptr;
	}

	void Initialize() {
		device = FCCS::RHI::CreateRHIDevice();
		queue = device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
		swapchain = FCCS::RHI::CreateSwapChain(queue, &desc);
		device->CreateTextureFromFile(L"C:\\Users\\42937\\Desktop\\pycode\\1.jpg", 800, 600);
	}
	void Update() {
		swapchain->Present();
	}
	void Release() {
		FCCS::RHI::DestroyRHIObject(device);
		FCCS::RHI::DestroyRHIObject(queue);
		FCCS::RHI::DestroyRHIObject(swapchain);
	}

	FCCS::RHI::Device* device;
	FCCS::RHI::CommandQueue* queue;
	FCCS::RHI::SwapChain *swapchain;
	FCCS::RHI::FCCS_SWAP_CHAIN_DESC desc;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	unsigned width = 800, height = 600;
	constexpr unsigned bufferCount = 3;
	auto window = FCCS::CreateWindowExW(L"fccs", width, height);
	FCCS::RHI::FCCS_SWAP_CHAIN_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.Hwnd = window->GetHWND();
	desc.BufferCount = bufferCount;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Context context(&desc);
	return window->Run(&context);
}
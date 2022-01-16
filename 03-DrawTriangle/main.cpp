#include <Windows.h>
#include <fccs/fccs.h>
#include <d3dx12.h>
#include <wrl/client.h>

class Callback : public fccs::window::IWindowCallback {
public:
	Callback(HWND hwnd) :hwnd(hwnd) {}
	void Initialize() {
		device = fccs::rhi::CreateDeivce();
		queue = device->CreateCommandQueue(fccs::rhi::CommandQueueType::Graphics);
		fccs::window::SwapChainDesc swapchainDesc = {};
		swapchainDesc.width = 800;
		swapchainDesc.height = 600;
		swapchainDesc.hwnd = hwnd;
		swapchain = fccs::window::CreateSwapChain(swapchainDesc, queue.get());

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
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

#include <Windows.h>
#include <FCCS.h>
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include <d3dx12.h>
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
		desc.DepthStencilFormat = DXGI_FORMAT_D32_FLOAT;
		FCCS::RHI::CreateDeviceAndSwapChain(&desc, &device, &swapchain);
		list = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
		queue = device->GetDefaultCommandQueue();

		vs = FCCS::RHI::CompileShaderFromFile(L"shader.hlsl", "VSMain", "vs_5_1");
		ps = FCCS::RHI::CompileShaderFromFile(L"shader.hlsl", "PSMain", "ps_5_1");

		rootsignature = device->CreateRootSignature(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = rootsignature->GetRootSignaturePtr();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBlobPtr());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBlobPtr());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;

		float m_aspectRatio = static_cast<float>(width) / height;
		float triangleVertices[3][4] = {
			{ 0.0f, 0.25f * m_aspectRatio, 0.0f,1.0f },
			{ 0.25f, -0.25f * m_aspectRatio, 0.0f,1.0f },
			{ -0.25f, -0.25f * m_aspectRatio, 0.0f,1.0f }
		};
		buffer = device->CreateStaticBuffer(triangleVertices, sizeof(triangleVertices));

		queue = device->GetDefaultCommandQueue();
	}
	void Update() {
		auto d3dlist = list->GetCommandListPtr();
		auto d3dqueue = queue->GetCommandQueuePtr();
		ID3D12CommandList* pLists[1] = { d3dlist };
		list->Reset(pso);

		d3dlist->Close();
		d3dqueue->ExecuteCommandLists(1, pLists);
		swapchain->Present();
	}
	void Release() {
		FCCS::RHI::DestroyRHIObject(device);
		FCCS::RHI::DestroyRHIObject(swapchain);
		FCCS::RHI::DestroyRHIObject(list);
		FCCS::RHI::DestroyRHIObject(rootsignature);
		FCCS::RHI::DestroyRHIObject(pso);
		FCCS::RHI::DestroyRHIObject(vs);
		FCCS::RHI::DestroyRHIObject(ps);
		FCCS::RHI::DestroyRHIObject(buffer);
	}

	HWND hwnd;

	FCCS::RHI::Device* device = nullptr;
	FCCS::RHI::SwapChain* swapchain = nullptr;
	FCCS::RHI::CommandList* list = nullptr;
	FCCS::RHI::CommandQueue* queue = nullptr;
	FCCS::RHI::RootSignature* rootsignature = nullptr;
	FCCS::RHI::PSO* pso = nullptr;
	FCCS::RHI::Blob* vs = nullptr;
	FCCS::RHI::Blob* ps = nullptr;
	FCCS::RHI::StaticBuffer* buffer = nullptr;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", width, height);
	Context context(window->GetHWND());
	return window->Run(&context);
}
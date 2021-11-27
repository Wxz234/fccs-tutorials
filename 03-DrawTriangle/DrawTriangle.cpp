#include <Windows.h>
#include <FCCS.h>
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include <d3dx12.h>
struct Global {
	inline static DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	inline static DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D32_FLOAT;
	inline static unsigned Width = 800;
	inline static unsigned Height = 600;
	inline static unsigned BufferCount = 3;
};

class Context : public FCCS::WindowContext {
public:
	Context(HWND hwnd) {
		desc.Width = Global::Width;
		desc.Height = Global::Height;
		desc.Hwnd = hwnd;
		desc.BufferCount = Global::BufferCount;
		desc.Format = Global::Format;
		desc.DepthStencilFormat = Global::DepthStencilFormat;
		device = nullptr;
		swapchain = nullptr;
		vs = nullptr;
		ps = nullptr;
		rootsignature = nullptr;
		pso = nullptr;
		list = nullptr;
		buffer = nullptr;
	}

	void Initialize() {
		FCCS::RHI::CreateDeviceAndSwapChain(&desc, &device, &swapchain);
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
		psoDesc.RTVFormats[0] = Global::Format;
		psoDesc.DSVFormat = Global::DepthStencilFormat;
		psoDesc.SampleDesc.Count = 1;
		pso = device->CreateGraphicsPSO(&psoDesc);
		
		list = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
		
		float m_aspectRatio = static_cast<float>(Global::Width) / static_cast<float>(Global::Height);
		float triangleVertices[3][4] =
		{
			{ 0.0f, 0.25f * m_aspectRatio, 0.0f,1.0f },
			{ 0.25f, -0.25f * m_aspectRatio, 0.0f,1.0f },
			{ -0.25f, -0.25f * m_aspectRatio, 0.0f ,1.0f}
		};
		buffer = device->CreateStaticBuffer(triangleVertices, sizeof(triangleVertices));
	}
	void Update() {
		swapchain->Present();
	}
	void Release() {
		FCCS::RHI::DestroyRHIObject(device);
		FCCS::RHI::DestroyRHIObject(swapchain);
		FCCS::RHI::DestroyRHIObject(vs);
		FCCS::RHI::DestroyRHIObject(ps);
		FCCS::RHI::DestroyRHIObject(rootsignature);
		FCCS::RHI::DestroyRHIObject(pso);
		FCCS::RHI::DestroyRHIObject(list);
		FCCS::RHI::DestroyRHIObject(buffer);
	}

	FCCS::FCCS_SWAP_CHAIN_DESC desc;

	FCCS::RHI::Device* device;
	FCCS::RHI::SwapChain* swapchain;
	FCCS::RHI::Blob* vs;
	FCCS::RHI::Blob* ps;
	FCCS::RHI::RootSignature* rootsignature;
	FCCS::RHI::PSO* pso;
	FCCS::RHI::CommandList* list;
	FCCS::RHI::StaticBuffer* buffer;
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", Global::Width, Global::Height);
	Context context(window->GetHWND());
	return window->Run(&context);
}
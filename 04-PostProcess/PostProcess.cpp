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
		for(unsigned i = 0;i < bufferCount; ++i) {
			list[i] = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
		}

		FCCS::RHI::Blob* vs = FCCS::RHI::CompileShaderFromFile(L"PostProcess.hlsl", "VSMain", "vs_5_1");
		FCCS::RHI::Blob* ps = FCCS::RHI::CompileShaderFromFile(L"PostProcess.hlsl", "PSMain", "ps_5_1");

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
		pso = device->CreateGraphicsPSO(&psoDesc);

		vs->Release();
		ps->Release();

		float m_aspectRatio = static_cast<float>(width) / height;
		float triangleVertices[3][4] = {
			{ 0.0f, 0.25f * m_aspectRatio, 0.0f,1.0f },
			{ 0.25f, -0.25f * m_aspectRatio, 0.0f,1.0f },
			{ -0.25f, -0.25f * m_aspectRatio, 0.0f,1.0f }
		};
		buffer = device->CreateStaticBuffer(triangleVertices, sizeof(triangleVertices));

		vertexBufferView = buffer->GetVertexBufferView(4 * sizeof(float), 12 * sizeof(float));
	}

	void Update() {
		auto frameIndex = swapchain->GetBackBufferIndex();
		auto d3dlist = list[frameIndex]->GetCommandListPtr();
		auto d3dqueue = device->GetDefaultCommandQueue()->GetCommandQueuePtr();
		auto viewport = CD3DX12_VIEWPORT(0.f, 0.f, width, height);
		auto scissorRect = CD3DX12_RECT(0, 0, width, height);
		ID3D12CommandList* pLists[1] = { d3dlist };
		list[frameIndex]->Reset(pso);

		d3dlist->SetGraphicsRootSignature(rootsignature->GetRootSignaturePtr());
		d3dlist->RSSetViewports(1, &viewport);
		d3dlist->RSSetScissorRects(1, &scissorRect);
		auto rtv = swapchain->GetRTV(frameIndex);
		auto dsv = swapchain->GetDSV();
		d3dlist->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(swapchain->GetResource(frameIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		d3dlist->ResourceBarrier(1, &barrier);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		d3dlist->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
		d3dlist->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		d3dlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dlist->IASetVertexBuffers(0, 1, &vertexBufferView);
		d3dlist->DrawInstanced(3, 1, 0, 0);
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(swapchain->GetResource(frameIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		d3dlist->ResourceBarrier(1, &barrier);
		d3dlist->Close();
		d3dqueue->ExecuteCommandLists(1, pLists);
		swapchain->Present();
	}
	void Release() {
		device->Release();
		swapchain->Release();
		for (unsigned i = 0; i < bufferCount; ++i) {
			list[i]->Release();
		}
		rootsignature->Release();
		pso->Release();
		buffer->Release();
	}

	HWND hwnd;

	FCCS::RHI::Device* device = nullptr;
	FCCS::RHI::SwapChain* swapchain = nullptr;
	FCCS::RHI::CommandList* list[bufferCount] = {};
	FCCS::RHI::RootSignature* rootsignature = nullptr;
	FCCS::RHI::PSO* pso = nullptr;
	FCCS::RHI::StaticBuffer* buffer = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	auto window = FCCS::CreateWindowExW(L"fccs", width, height);
	Context context(window->GetHWND());
	return window->Run(&context);
}
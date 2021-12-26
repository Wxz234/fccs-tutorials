#include <Windows.h>
#include <FCCS.h>
#include <vector>
#include <wrl.h>
#include <string>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#define D3DX12_NO_STATE_OBJECT_HELPERS
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include <directx/d3dx12.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr unsigned width = 800, height = 600;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);

	auto commandqueue = device->GetRenderCommandQueue();
	auto commandlist = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	std::vector<FCCS::CommandAllocator*> commandallocator;
	auto buffercount = swapchain->GetBufferCount();
	for (FCCS::uint32 i = 0; i < buffercount; ++i) {
		commandallocator.emplace_back(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT));
	}

	std::string vs_str = R"(
	float4 main( float4 pos : POSITION ) : SV_POSITION
	{
		return pos;
	})";
	std::string ps_str = R"(
	float4 main() : SV_TARGET
	{
		return float4(1.0f, 1.0f, 1.0f, 1.0f);
	})";
	Microsoft::WRL::ComPtr<ID3DBlob> vs_blob;
	Microsoft::WRL::ComPtr<ID3DBlob> ps_blob;
	D3DCompile(vs_str.c_str(), vs_str.size(), nullptr, nullptr, nullptr, "main", "vs_5_1", 0, 0, &vs_blob, nullptr);
	D3DCompile(ps_str.c_str(), ps_str.size(), nullptr, nullptr, nullptr, "main", "ps_5_1", 0, 0, &ps_blob, nullptr);

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	auto device_ptr = FCCS::Cast<ID3D12Device>(device->GetNativePtr());
	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		device_ptr->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	}

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, 1 };
	psoDesc.pRootSignature = rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = swapchain->GetRenderTargetFormat();
	psoDesc.SampleDesc.Count = 1;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	device_ptr->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelinestate));
	float ratio = (float)width / (float)height;
	float triangleVertices[3][4] =
	{
		{ 0.0f, 0.25f * ratio, 0.0f,1.f } ,
		{ 0.25f, -0.25f * ratio, 0.0f,1.f },
		{ -0.25f, -0.25f * ratio, 0.0f,1.f }
	};
	UINT vertexBufferSize = sizeof(triangleVertices);
	auto buffer = device->CreateBuffer(vertexBufferSize);
	buffer->Update(triangleVertices, vertexBufferSize);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = buffer->GetAddress();
	vertexBufferView.StrideInBytes = sizeof(float) * 4;
	vertexBufferView.SizeInBytes = vertexBufferSize;

	CD3DX12_VIEWPORT m_viewport(0.F, 0.F, width, height);
	CD3DX12_RECT m_scissorRect(0, 0, width, height);
	while (window->IsRun()) {
		auto frameIndex = swapchain->GetCurrentBackBufferIndex();
		auto cmdalloc_ptr = FCCS::Cast<ID3D12CommandAllocator>(commandallocator[frameIndex]->GetNativePtr());
		auto cmdlist_ptr = FCCS::Cast<ID3D12GraphicsCommandList>(commandlist->GetNativePtr());
		commandallocator[frameIndex]->Reset();
		cmdlist_ptr->Reset(cmdalloc_ptr, pipelinestate.Get());
		cmdlist_ptr->SetGraphicsRootSignature(rootSignature.Get());
		cmdlist_ptr->RSSetViewports(1, &m_viewport);
		cmdlist_ptr->RSSetScissorRects(1, &m_scissorRect);

		auto swapchain_ptr = FCCS::Cast<IDXGISwapChain>(swapchain->GetNativePtr());
		Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget;
		swapchain_ptr->GetBuffer(frameIndex, IID_PPV_ARGS(&renderTarget));
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		// Indicate that the back buffer will be used as a render target.
		cmdlist_ptr->ResourceBarrier(1, &barrier);

		auto rtvHandle = swapchain->GetRenderTargetView(frameIndex);
		cmdlist_ptr->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// Record commands.
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		cmdlist_ptr->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		cmdlist_ptr->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdlist_ptr->IASetVertexBuffers(0, 1, &vertexBufferView);
		cmdlist_ptr->DrawInstanced(3, 1, 0, 0);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		// Indicate that the back buffer will now be used to present.
		cmdlist_ptr->ResourceBarrier(1, &barrier);

		commandlist->Close();
		commandqueue->Execute(commandlist);
		swapchain->Present(1);
	}

	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	FCCS::DestroyFObject(commandlist);
	FCCS::DestroyFObject(buffer);
	for (auto alloc : commandallocator) {
		FCCS::DestroyFObject(alloc);
	}
	return 0;
}
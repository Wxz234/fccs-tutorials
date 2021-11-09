#include <FCCS.h>
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include <d3dx12.h>
#include <Windows.h>
#include <d3dcompiler.h>
#include "Vertex.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
using namespace FCCS;
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr uint32 frameCount = 3;
	constexpr DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, ds_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	constexpr uint32 width = 800, height = 600;


	RefCountPtr<IWindow> window;
	CreateWindowExW(L"fccs", width, height, &window);

	RefCountPtr<Graphics::IDevice> device;
	Graphics::CreateDevice(&device);
	auto device_ptr = reinterpret_cast<ID3D12Device*>(device->GetNativeObject());

	RefCountPtr<Graphics::ICommandQueue> comandqueue;
	device->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT,&comandqueue);

	Graphics::FCCS_SWAP_CHAIN_DESC desc{};
	desc.BufferCount = frameCount;
	desc.Width = 800;
	desc.Height = 600;
	desc.Format = format;
	desc.DepthStencilFormat = ds_format;
	RefCountPtr<Graphics::ISwapChain> swapchain;
	Graphics::CreateSwapChain(comandqueue.Get(), window.Get(), &desc, &swapchain);
	
	RefCountPtr<Graphics::ICommandList> commandlists[frameCount];
	RefCountPtr<Graphics::IGpuResource> swapchainbuffer[frameCount];
	ID3D12GraphicsCommandList* list_ptr[frameCount];
	for (uint32 i = 0; i < frameCount; ++i) {
		device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &commandlists[i]);
		swapchain->GetBuffer(i, &swapchainbuffer[i]);
		list_ptr[i] = reinterpret_cast<ID3D12GraphicsCommandList*>(commandlists[i]->GetNativeObject());
	}

	CD3DX12_DESCRIPTOR_RANGE1 ranges;
	CD3DX12_ROOT_PARAMETER1 rootParameters;

	ranges.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	rootParameters.InitAsDescriptorTable(1, &ranges);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1(1, &rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	RefCountPtr<ID3D12RootSignature> rootSignature;
	RefCountPtr<ID3DBlob> signature;
	RefCountPtr<ID3DBlob> error;
	D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &signature, &error);
	device_ptr->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	RefCountPtr<ID3DBlob> vertexShader;
	RefCountPtr<ID3DBlob> pixelShader;
	D3DCompileFromFile(L"vs.hlsl", nullptr, nullptr, "main", "vs_5_1", 0, 0, &vertexShader, nullptr);
	D3DCompileFromFile(L"ps.hlsl", nullptr, nullptr, "main", "ps_5_1", 0, 0, &pixelShader, nullptr);
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	Graphics::FCCS_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
	pipelineDesc.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
	pipelineDesc.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	pipelineDesc.SetRenderTargetFormats(1, &format, ds_format);
	pipelineDesc.SetInputLayout(_countof(inputElementDescs), inputElementDescs);
	pipelineDesc.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	pipelineDesc.SetSampleMask(0xFFFFFFFF);
	pipelineDesc.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineDesc.SetVertexShader(CD3DX12_SHADER_BYTECODE(vertexShader.Get()));
	pipelineDesc.SetPixelShader(CD3DX12_SHADER_BYTECODE(pixelShader.Get()));
	pipelineDesc.SetRootSignature(rootSignature.Get());
	
	RefCountPtr<Graphics::IPipelineState> pipeline;
	device->CreateGraphicsPipelineState(&pipelineDesc, &pipeline);

	Vertex triangleVertices[] =
	{
		{ { 0.0f,0.5f, 1.0f} },
		{ { 0.25f, -0.5f,0.9f } },
		{ { -0.25f, -0.5f, 0.9f } }
	};
	RefCountPtr<Graphics::IGpuResource> resource;
	RefCountPtr<Graphics::IGpuResource> camera_resource;
	device->CreateBuffer(D3D12_HEAP_TYPE_UPLOAD, sizeof(triangleVertices), D3D12_RESOURCE_STATE_GENERIC_READ, &resource);
	void* res_address = resource->Map();
	MemCopyU64(res_address, triangleVertices, sizeof(triangleVertices));
	resource->Unmap();
	auto vbv = resource->GetVertexBufferView(sizeof(Vertex), sizeof(triangleVertices));

	Camera camera;
	device->CreateBuffer(D3D12_HEAP_TYPE_UPLOAD, 100, D3D12_RESOURCE_STATE_GENERIC_READ, &camera_resource);
	void* cb_res = camera_resource->Map();
	
	D3D12_VIEWPORT viewport{};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = static_cast<LONG>(width);
	scissorRect.bottom = static_cast<LONG>(height);


	while (window->IsActive()) {
		const uint32 frameIndex = swapchain->GetCurrentBackBufferIndex();
		commandlists[frameIndex]->Reset(pipeline.Get());
		commandlists[frameIndex]->SetGraphicsRootSignature(rootSignature.Get());
		list_ptr[frameIndex]->RSSetViewports(1, &viewport);
		list_ptr[frameIndex]->RSSetScissorRects(1, &scissorRect);
		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		auto rtvHandle = swapchain->GetRTVHandle(frameIndex);
		auto dsvHandle = swapchain->GetDSVHandle();
		list_ptr[frameIndex]->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		list_ptr[frameIndex]->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		list_ptr[frameIndex]->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
		list_ptr[frameIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		list_ptr[frameIndex]->IASetVertexBuffers(0, 1, &vbv);
		list_ptr[frameIndex]->DrawInstanced(3, 1, 0, 0);

		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandlists[frameIndex]->Close();
		comandqueue->Execute(commandlists[frameIndex].Get());

		swapchain->Present();
	}
	comandqueue->WaitIdle();
	return 0;
}
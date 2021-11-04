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
	constexpr DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	RefCountPtr<IWindow> window;
	CreateWindowExW(L"fccs", 800, 600, &window);

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
	RefCountPtr<Graphics::ISwapChain> swapchain;
	Graphics::CreateSwapChain(comandqueue.Get(), window.Get(), &desc, &swapchain);
	
	RefCountPtr<Graphics::ICommandList> commandlists[frameCount];
	RefCountPtr<Graphics::IGpuResource> swapchainbuffer[frameCount];
	for (uint32 i = 0; i < frameCount; ++i) {
		device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT, &commandlists[i]);
		swapchain->GetBuffer(i, &swapchainbuffer[i]);
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	RefCountPtr<ID3D12RootSignature> rootSignature;
	RefCountPtr<ID3DBlob> signature;
	RefCountPtr<ID3DBlob> error;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &error);
	device_ptr->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));


	RefCountPtr<ID3DBlob> vertexShader;
	RefCountPtr<ID3DBlob> pixelShader;
	D3DCompileFromFile(L"vs.hlsl", nullptr, nullptr, "main", "vs_5_1", 0, 0, &vertexShader, nullptr);
	D3DCompileFromFile(L"ps.hlsl", nullptr, nullptr, "main", "ps_5_1", 0, 0, &pixelShader, nullptr);
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	Graphics::FCCS_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
	pipelineDesc.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
	pipelineDesc.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	pipelineDesc.SetRenderTargetFormats(1, &format, DXGI_FORMAT_UNKNOWN);
	pipelineDesc.SetInputLayout(_countof(inputElementDescs), inputElementDescs);
	pipelineDesc.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	pipelineDesc.SetSampleMask(0xFFFFFFFF);
	pipelineDesc.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineDesc.SetVertexShader(CD3DX12_SHADER_BYTECODE(vertexShader.Get()));
	pipelineDesc.SetPixelShader(CD3DX12_SHADER_BYTECODE(pixelShader.Get()));
	pipelineDesc.SetRootSignature(rootSignature.Get());

	RefCountPtr<Graphics::IPipelineState> _pipeline;
	device->CreateGraphicsPipelineState(&pipelineDesc, &_pipeline);
	while (window->IsActive()) {
		const uint32 frameIndex = swapchain->GetCurrentBackBufferIndex();
		commandlists[frameIndex]->Reset(nullptr);
		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);




		commandlists[frameIndex]->ResourceBarrier(swapchainbuffer[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandlists[frameIndex]->Close();
		comandqueue->Execute(commandlists[frameIndex].Get());
		swapchain->Present();
	}
	return 0;
}
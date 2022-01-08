#include <Windows.h>
#include <FCCS.h>
#include "d3dx12.h"
//shader
#include "vs.h"
#include "ps.h"
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr unsigned width = 400, height = 400;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);
	auto commandqueue = device->GetRenderCommandQueue();
	auto commandlist = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	auto commandallocator = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	auto rootsignature = device->CreateRootSignature(&rootSignatureDesc);

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, 1 };
	psoDesc.pRootSignature = FCCS::Cast<ID3D12RootSignature>(rootsignature->GetNativePtr());
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs_blob, sizeof(vs_blob));
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps_blob, sizeof(ps_blob));
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = swapchain->GetRenderTargetFormat();
	psoDesc.SampleDesc.Count = 1;
	auto pipelinestate = device->CreateGraphicsPipelineState(&psoDesc);

	float triangleVertices[3][4] = {
		{ 0.00f, 0.25f, 0.00f, 1.00f },
		{ 0.25f,-0.25f, 0.00f, 1.00f },
		{-0.25f,-0.25f, 0.00f, 1.00f }, 
	};
	auto buffer = device->CreateStaticGpuBuffer(triangleVertices, sizeof(triangleVertices));
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = buffer->GetAddress();
	vertexBufferView.StrideInBytes = sizeof(float) * 4;
	vertexBufferView.SizeInBytes = sizeof(triangleVertices);

	CD3DX12_VIEWPORT m_viewport(0.f, 0.f, width, height);
	CD3DX12_RECT m_scissorRect(0, 0, width, height);

	while (window->IsRun()) {
		auto frameIndex = swapchain->GetCurrentBackBufferIndex();
		auto cmdalloc_ptr = FCCS::Cast<ID3D12CommandAllocator>(commandallocator->GetNativePtr());
		auto cmdlist_ptr = FCCS::Cast<ID3D12GraphicsCommandList>(commandlist->GetNativePtr());

		commandallocator->Reset();
		commandlist->Reset(commandallocator, pipelinestate);
		cmdlist_ptr->SetGraphicsRootSignature(FCCS::Cast<ID3D12RootSignature>(rootsignature->GetNativePtr()));
		cmdlist_ptr->RSSetViewports(1, &m_viewport);
		cmdlist_ptr->RSSetScissorRects(1, &m_scissorRect);

		auto texture = swapchain->GetRenderTargetTexture(frameIndex);
		commandlist->ResourceBarrier(texture, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		auto rtvHandle = swapchain->GetRenderTargetView(frameIndex);
		cmdlist_ptr->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		cmdlist_ptr->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdlist_ptr->IASetVertexBuffers(0, 1, &vertexBufferView);
		cmdlist_ptr->DrawInstanced(3, 1, 0, 0);

		commandlist->ResourceBarrier(texture, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandlist->Close();
		commandqueue->Execute(commandlist);
		swapchain->Present(1);
	}

	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	FCCS::DestroyFObject(commandlist);
	FCCS::DestroyFObject(buffer);
	FCCS::DestroyFObject(commandallocator);
	FCCS::DestroyFObject(pipelinestate);
	FCCS::DestroyFObject(rootsignature);
	return 0;
}
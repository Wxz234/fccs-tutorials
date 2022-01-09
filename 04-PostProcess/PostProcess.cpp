#include <Windows.h>
#include <FCCS.h>
#include <vector>
#include "d3dx12.h"
//shader
#include "vs.h"
#include "ps.h"
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr unsigned width = 1000, height = 1000;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);
	auto commandqueue = device->GetRenderCommandQueue();
	auto commandlist = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	std::vector<FCCS::CommandAllocator*> commandallocator;
	for (FCCS::uint32 i = 0; i < swapchain->GetBufferCount(); ++i) {
		commandallocator.push_back(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT));
	}

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
		{ 0.00f, 1.00f, 0.00f, 1.00f },
		{ 1.00f,-1.00f, 0.00f, 1.00f },
		{-0.25f,-0.25f, 0.00f, 1.00f },
	};
	auto buffer = device->CreateStaticGpuBuffer(triangleVertices, sizeof(triangleVertices));
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = buffer->GetAddress();
	vertexBufferView.StrideInBytes = sizeof(float) * 4;
	vertexBufferView.SizeInBytes = sizeof(triangleVertices);

	CD3DX12_VIEWPORT m_viewport(0.f, 0.f, width, height);
	CD3DX12_RECT m_scissorRect(0, 0, width, height);

	auto tex_desc = CD3DX12_RESOURCE_DESC::Tex2D(swapchain->GetRenderTargetFormat(), width, height);
	tex_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = swapchain->GetRenderTargetFormat();
	clearValue.Color[0] = clearValue.Color[1] = clearValue.Color[2] = clearValue.Color[3] = 0;
	auto render_texture = device->CreateTexture(&tex_desc, &clearValue);

	auto postprocess = FCCS::CreatePostProcess(device, swapchain->GetRenderTargetFormat(), swapchain->GetDepthStencilFormat(), FCCS::Effect::FXAA);

	while (window->IsRun()) {
		auto frameIndex = swapchain->GetCurrentBackBufferIndex();
		auto cmdalloc_ptr = FCCS::Cast<ID3D12CommandAllocator>(commandallocator[frameIndex]->GetNativePtr());
		auto cmdlist_ptr = FCCS::Cast<ID3D12GraphicsCommandList>(commandlist->GetNativePtr());

		commandallocator[frameIndex]->Reset();
		commandlist->Reset(commandallocator[frameIndex], pipelinestate);
		cmdlist_ptr->SetGraphicsRootSignature(FCCS::Cast<ID3D12RootSignature>(rootsignature->GetNativePtr()));
		cmdlist_ptr->RSSetViewports(1, &m_viewport);
		cmdlist_ptr->RSSetScissorRects(1, &m_scissorRect);

		commandlist->ResourceBarrier(render_texture, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdlist_ptr->OMSetRenderTargets(1, &render_texture->GetRenderTargetView(), FALSE, nullptr);
		cmdlist_ptr->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdlist_ptr->IASetVertexBuffers(0, 1, &vertexBufferView);
		cmdlist_ptr->DrawInstanced(3, 1, 0, 0);
		commandlist->ResourceBarrier(render_texture, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		auto texture = swapchain->GetRenderTargetTexture(frameIndex);
		commandlist->ResourceBarrier(texture, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		auto rtvHandle = swapchain->GetRenderTargetView(frameIndex);
		cmdlist_ptr->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		postprocess->SetTexture(render_texture);
		postprocess->Process(commandlist);

		commandlist->ResourceBarrier(texture, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandlist->ResourceBarrier(render_texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COMMON);
		commandlist->Close();
		commandqueue->Execute(commandlist);
		swapchain->Present(1);
	}

	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	FCCS::DestroyFObject(commandlist);
	FCCS::DestroyFObject(buffer);
	FCCS::DestroyFObject(pipelinestate);
	FCCS::DestroyFObject(rootsignature);
	FCCS::DestroyFObject(render_texture);
	for (auto allocator : commandallocator) {
		FCCS::DestroyFObject(allocator);
	}
	return 0;
}
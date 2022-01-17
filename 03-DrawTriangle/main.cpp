#include <Windows.h>
#include <fccs/fccs.h>
#include <d3dx12.h>
#include <wrl/client.h>
#include <cstring>
#include "vs.h"
#include "ps.h"

class Callback : public fccs::window::IWindowCallback {
public:
	Callback(HWND hwnd) :hwnd(hwnd) {}
	void Initialize() {
		device = fccs::rhi::CreateDeivce();
		queue = device->CreateCommandQueue(fccs::rhi::CommandQueueType::Graphics);
		list = device->CreateCommandList(fccs::rhi::CommandQueueType::Graphics);
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
		auto device_ptr = (ID3D12Device*)device->GetNativePtr();
		device_ptr->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, 1 };
		psoDesc.pRootSignature = rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs_blob, sizeof(vs_blob));
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps_blob, sizeof(ps_blob));
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;

		device_ptr->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));

		float triangleVertices[3][4] = {
			{ 0.00f, 0.25f, 0.00f, 1.00f },
			{ 0.25f,-0.25f, 0.00f, 1.00f },
			{-0.25f,-0.25f, 0.00f, 1.00f },
		};

		auto buffer_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto buffe_desc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(triangleVertices));
		device_ptr->CreateCommittedResource(&buffer_prop, D3D12_HEAP_FLAG_NONE, &buffe_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertex));

		void* myVertex = nullptr;
		vertex->Map(0, nullptr, &myVertex);
		memcpy(myVertex, triangleVertices, sizeof(triangleVertices));

		vertexBufferView.BufferLocation = vertex->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = sizeof(float) * 4;
		vertexBufferView.SizeInBytes = sizeof(triangleVertices);
	}
	void Update() {
		list->Open();
		auto m_commandList = (ID3D12GraphicsCommandList*)(list->GetNativePtr());
		auto frameIndex = swapchain->GetCurrentBackBufferIndex();
		Microsoft::WRL::ComPtr<ID3D12Resource>  m_renderTargets = swapchain->GetNativeResourcePtr(frameIndex);
		// Transition the render target into the correct state to allow for drawing into it.
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->ResourceBarrier(1, &barrier);

		auto rtvDescriptor = swapchain->GetRenderTargetView(frameIndex);
		m_commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

		D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(800), static_cast<float>(600), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		D3D12_RECT scissorRect = { 0, 0, static_cast<LONG>(800), static_cast<LONG>(600) };
		m_commandList->RSSetViewports(1, &viewport);
		m_commandList->RSSetScissorRects(1, &scissorRect);

		m_commandList->SetGraphicsRootSignature(rootSignature.Get());
		m_commandList->SetPipelineState(pso.Get());
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		m_commandList->DrawInstanced(3, 1, 0, 0);

		barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_commandList->ResourceBarrier(1, &barrier);

		list->Close();
		fccs::rhi::ICommandList* lists[] = { list.get() };
		queue->ExecuteCommandLists(1, lists);
		swapchain->Present(1);
	}
	void Release() {}
private:
	HWND hwnd;
	fccs::rhi::DeviceHandle device;
	fccs::rhi::CommandQueueHandle queue;
	fccs::rhi::CommandListHandle list;
	fccs::window::SwapChainHandle swapchain;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertex;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
};


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	auto window = fccs::window::CreateFCCSWindow();
	window->OpenWindow();
	Callback callback(window->GetHWND());
	return window->Run(&callback);
}

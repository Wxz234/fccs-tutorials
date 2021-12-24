#include <Windows.h>
#include <FCCS.h>
#include <vector>
#include <string>
#include <cstring>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <wrl.h>
#define D3DX12_NO_STATE_OBJECT_HELPERS
#define D3DX12_NO_CHECK_FEATURE_SUPPORT_CLASS
#include "d3dx12.h"
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	constexpr unsigned width = 800, height = 600;
	auto window = FCCS::CreateFCCSWindow(L"fccs", width, height);
	auto device = FCCS::CreateDevice(0);
	auto swapchain = FCCS::CreateSwapChain(device, window);
	auto buffercount = swapchain->GetBufferCount();

	auto commandqueue = device->GetRenderCommandQueue();
	auto commandlist = device->CreateCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	std::vector<FCCS::CommandAllocator*> commandallocator;
	for (FCCS::uint32 i = 0;i < buffercount; ++i) {
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

	Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
	auto _device = FCCS::Cast<ID3D12Device>(device->GetNativePtr());
	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
	}

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, 1 };
	psoDesc.pRootSignature = _rootSignature.Get();
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
	_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelinestate));
	float ratio = (float)width / (float)height;
	float triangleVertices[3][4] =
	{
		{ 0.0f, 0.25f * ratio, 0.0f,1.f } ,
		{ 0.25f, -0.25f * ratio, 0.0f,1.f },
		{ -0.25f, -0.25f * ratio, 0.0f,1.f } 
	};
	const FCCS::uint32 vertexBufferSize = sizeof(triangleVertices);
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer;
	_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));
	void* pVertexDataBegin;
	_vertexBuffer->Map(0,nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
	memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
	_vertexBuffer->Unmap(0, nullptr);
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(triangleVertices);
	_vertexBufferView.SizeInBytes = vertexBufferSize;
	while (window->IsRun()) {
		swapchain->Present(1);
	}

	FCCS::DestroyFObject(swapchain);
	FCCS::DestroyFObject(device);
	FCCS::DestroyFObject(window);
	FCCS::DestroyFObject(commandlist);
	for (auto alloc : commandallocator) {
		FCCS::DestroyFObject(alloc);
	}
	return 0;
}
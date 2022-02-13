//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#include "Metalib.h"
#include "stdafx.h"
#include "D3D12SimpleBox.h"
#include <DXRuntime/FrameResource.h>
D3D12SimpleBox::D3D12SimpleBox(uint32_t width, uint32_t height, std::wstring name)
	: DXSample(width, height, name),
	  m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	  m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
	  m_rtvDescriptorSize(0) {
}
void D3D12SimpleBox::OnInit() {
	LoadPipeline();
	LoadAssets();
}

// Load the rendering pipeline dependencies.
void D3D12SimpleBox::LoadPipeline() {
	// create device;
	device = std::make_unique<Device>();
	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(device->DxDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(device->DxgiFactory()->CreateSwapChainForHwnd(
		m_commandQueue.Get(),// Swap chain needs the queue so that it can force a flush on it.
		Win32Application::GetHwnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain));

	// This sample does not support fullscreen transitions.
	ThrowIfFailed(device->DxgiFactory()->MakeWindowAssociation(Win32Application::GetHwnd(), DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain.As(&m_swapChain));
	m_backBufferIndex = 0;

	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->DxDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

		m_rtvDescriptorSize = device->DxDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (uint32_t n = 0; n < FrameCount; n++) {
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			device->DxDevice()->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}
	// Init FrameResources
	for (auto&& i : frameResources) {
		i = std::unique_ptr<FrameResource>(new FrameResource(device.get()));
	}
}

// Load the sample assets.
void D3D12SimpleBox::LoadAssets() {
	// Create triangle mesh
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ThrowIfFailed(
		device
			->DxDevice()
			->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
	ThrowIfFailed(device->DxDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
	ThrowIfFailed(commandList->Close());
	ThrowIfFailed(cmdAllocator->Reset());
	ThrowIfFailed(commandList->Reset(cmdAllocator.Get(), nullptr));
	std::unique_ptr<UploadBuffer> uploadBuffer;
	{
		// Create a vertex sample
		static Vertex vertexSample;
		std::vector<vbyte> vertexData(vertexSample.structSize * 3);
		vbyte* vertexDataPtr = vertexData.data();
		// Vertex 0
		vertexSample.position.Get(vertexDataPtr) = {0.0f, 0.25f * m_aspectRatio, 0.0f};
		vertexSample.color.Get(vertexDataPtr) = {1.0f, 0.0f, 0.0f, 1.0f};
		vertexDataPtr += vertexSample.structSize;
		// Vertex 1
		vertexSample.position.Get(vertexDataPtr) = {0.25f, -0.25f * m_aspectRatio, 0.0f};
		vertexSample.color.Get(vertexDataPtr) = {0.0f, 1.0f, 0.0f, 1.0f};
		vertexDataPtr += vertexSample.structSize;
		// Vertex 2
		vertexSample.position.Get(vertexDataPtr) = {-0.25f, -0.25f * m_aspectRatio, 0.0f};
		vertexSample.color.Get(vertexDataPtr) = {0.0f, 0.0f, 1.0f, 1.0f};
		// Create uploadBuffer
		uploadBuffer = std::make_unique<UploadBuffer>(
			device.get(),
			vertexData.size());
		uploadBuffer->CopyData(0, vertexData);
		// Generate mesh
		std::vector<rtti::Struct const*> structs;
		structs.emplace_back(&vertexSample);
		triangleMesh = std::make_unique<Mesh>(
			device.get(),
			structs,
			3);
		// Copy uploadBuffer to mesh
		commandList->CopyBufferRegion(
			triangleMesh->VertexBuffers()[0].GetResource(),
			0,
			uploadBuffer->GetResource(),
			0,
			vertexData.size());
	}
	ThrowIfFailed(commandList->Close());
	// Execute CommandList
	ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
	m_commandQueue->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
	// Create an empty root signature.
	{
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed(device->DxDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	// Create the pipeline state, which includes compiling and loading shaders.
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		// Enable better shader debugging with the graphics debugging tools.
		uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		uint32_t compileFlags = 0;
#endif
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shader/shaders.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
		ThrowIfFailed(D3DCompileFromFile(GetAssetFullPath(L"shader/shaders.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

		// Define the vertex input layout.

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		auto meshLayout = triangleMesh->Layout();
		psoDesc.InputLayout = {meshLayout.data(), uint(meshLayout.size())};
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		ThrowIfFailed(device->DxDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
	}

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
		ThrowIfFailed(device->DxDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValue = 1;
		m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
		if (m_fence->GetCompletedValue() < m_fenceValue) {
			LPCWSTR falseValue = 0;
			HANDLE eventHandle = CreateEventEx(nullptr, falseValue, false, EVENT_ALL_ACCESS);
			ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValue, eventHandle));
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}

// Update frame-based values.
void D3D12SimpleBox::OnUpdate() {
}

// Render the scene.
void D3D12SimpleBox::OnRender() {
	// Record all the commands we need to render the scene into the command list.
	auto curFrame = m_backBufferIndex;
	auto nextFrame = (curFrame + 1) % FrameCount;
	auto lastFrame = (nextFrame + 1) % FrameCount;
	// Execute and Present
	frameResources[curFrame]->Execute(
		m_commandQueue.Get(),
		m_fence.Get(),
		m_fenceValue);
	ThrowIfFailed(m_swapChain->Present(0, 0));
	m_backBufferIndex = (m_backBufferIndex + 1) % FrameCount;
	// Signal Frame
	frameResources[curFrame]->Signal(
		m_commandQueue.Get(),
		m_fence.Get());
	// Populate next frame
	PopulateCommandList(frameResources[nextFrame]->Command(), nextFrame);
	// Sync last frame
	frameResources[lastFrame]->Sync(
		m_fence.Get());

	// Present the frame.
}

void D3D12SimpleBox::OnDestroy() {
	// Sync all frame
	for (auto&& i : frameResources) {
		i->Sync(m_fence.Get());
	}
}

void D3D12SimpleBox::PopulateCommandList(CommandListHandle const& cmdListHandle, uint frameIndex) const {

	auto cmdList = cmdListHandle.CmdList();
	// Set necessary state.
	cmdList->SetGraphicsRootSignature(m_rootSignature.Get());
	cmdList->RSSetViewports(1, &m_viewport);
	cmdList->RSSetScissorRects(1, &m_scissorRect);

	// Indicate that the back buffer will be used as a render target.
	cmdList->ResourceBarrier(1, get_rvalue_ptr(CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, m_rtvDescriptorSize);
	cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferView;
	triangleMesh->GetVertexBufferView(vertexBufferView);
	cmdList->IASetVertexBuffers(0, vertexBufferView.size(), vertexBufferView.data());
	cmdList->SetPipelineState(
		m_pipelineState.Get());
	cmdList->DrawInstanced(3, 1, 0, 0);

	// Indicate that the back buffer will now be used to present.
	cmdList->ResourceBarrier(1, get_rvalue_ptr(CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT)));
}
D3D12SimpleBox::~D3D12SimpleBox() {}

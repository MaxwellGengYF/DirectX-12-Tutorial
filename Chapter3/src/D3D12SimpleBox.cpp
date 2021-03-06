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
#include <Component/Camera.h>
#include <Resource/DefaultBuffer.h>
D3D12SimpleBox::D3D12SimpleBox(uint32_t width, uint32_t height, std::wstring name)
	: DXSample(width, height, name),
	  m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	  m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)) {
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
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->DxDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

		m_dsvDescriptorSize = device->DxDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}
	// Create render target
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		// Create a RTV for each frame.
		for (uint32_t n = 0; n < FrameCount; n++) {
			ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));

			device->DxDevice()->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, m_rtvDescriptorSize);
		}
	}
	// Create depth buffer
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC texDesc;
		memset(&texDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texDesc.Alignment = 0;
		texDesc.Width = m_scissorRect.right;
		texDesc.Height = m_scissorRect.bottom;
		texDesc.DepthOrArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		D3D12_CLEAR_VALUE defaultClearValue;
		defaultClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		defaultClearValue.DepthStencil.Depth = 1;
		ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_DEPTH_READ,
			&defaultClearValue,
			IID_PPV_ARGS(&m_depthTarget)));
		device->DxDevice()->CreateDepthStencilView(m_depthTarget.Get(), nullptr, dsvHandle);
	}
	// Init FrameResources
	for (auto&& i : frameResources) {
		i = std::unique_ptr<FrameResource>(new FrameResource(device.get()));
	}
}
static Vertex vertexSample;
// Cube model generated from Unity3D
static XMFLOAT3 vertices[] = {
	XMFLOAT3(0.5, -0.5, 0.5),
	XMFLOAT3(-0.5, -0.5, 0.5),
	XMFLOAT3(0.5, 0.5, 0.5),
	XMFLOAT3(-0.5, 0.5, 0.5),
	XMFLOAT3(0.5, 0.5, -0.5),
	XMFLOAT3(-0.5, 0.5, -0.5),
	XMFLOAT3(0.5, -0.5, -0.5),
	XMFLOAT3(-0.5, -0.5, -0.5),
	XMFLOAT3(0.5, 0.5, 0.5),
	XMFLOAT3(-0.5, 0.5, 0.5),
	XMFLOAT3(0.5, 0.5, -0.5),
	XMFLOAT3(-0.5, 0.5, -0.5),
	XMFLOAT3(0.5, -0.5, -0.5),
	XMFLOAT3(0.5, -0.5, 0.5),
	XMFLOAT3(-0.5, -0.5, 0.5),
	XMFLOAT3(-0.5, -0.5, -0.5),
	XMFLOAT3(-0.5, -0.5, 0.5),
	XMFLOAT3(-0.5, 0.5, 0.5),
	XMFLOAT3(-0.5, 0.5, -0.5),
	XMFLOAT3(-0.5, -0.5, -0.5),
	XMFLOAT3(0.5, -0.5, -0.5),
	XMFLOAT3(0.5, 0.5, -0.5),
	XMFLOAT3(0.5, 0.5, 0.5),
	XMFLOAT3(0.5, -0.5, 0.5)};
static uint indices[] = {0, 2, 3, 0, 3, 1, 8, 4, 5, 8, 5, 9, 10, 6, 7, 10, 7, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

static UploadBuffer* BuildCubeVertex(Device* device) {
	constexpr size_t VERTEX_COUNT = array_count(vertices);
	std::vector<vbyte> vertexData(vertexSample.structSize * VERTEX_COUNT);
	vbyte* vertexDataPtr = vertexData.data();
	for (size_t i = 0; i < VERTEX_COUNT; ++i) {
		XMFLOAT3 vert = vertices[i];
		vertexSample.position.Get(vertexDataPtr) = vert;
		XMFLOAT4 color(
			vert.x + 0.5f,
			vert.y + 0.5f,
			vert.z + 0.5f,
			1);
		vertexSample.color.Get(vertexDataPtr) = color;
		vertexDataPtr += vertexSample.structSize;
	}
	UploadBuffer* vertBuffer = new UploadBuffer(
		device,
		vertexData.size());
	vertBuffer->CopyData(0, vertexData);
	return vertBuffer;
}
static UploadBuffer* BuildCubeIndices(Device* device) {
	UploadBuffer* indBuffer = new UploadBuffer(
		device,
		array_count(indices) * sizeof(uint));
	indBuffer->CopyData(0, {reinterpret_cast<vbyte const*>(indices), array_count(indices) * sizeof(uint)});
	return indBuffer;
}
// Load the sample assets.
void D3D12SimpleBox::LoadAssets() {
	// Create mesh
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
	std::unique_ptr<UploadBuffer> vertexUpload(BuildCubeVertex(device.get()));
	std::unique_ptr<UploadBuffer> indexUpload(BuildCubeIndices(device.get()));
	std::vector<rtti::Struct const*> structs;
	structs.emplace_back(&vertexSample);
	triangleMesh = std::make_unique<Mesh>(
		device.get(),
		structs,
		array_count(vertices),
		array_count(indices));
	// Copy vertex buffer to mesh
	commandList->CopyBufferRegion(
		triangleMesh->VertexBuffers()[0].GetResource(),
		0,
		vertexUpload->GetResource(),
		0,
		vertexUpload->GetByteSize());
	// Copy index buffer to mesh
	commandList->CopyBufferRegion(
		triangleMesh->IndexBuffer().GetResource(),
		0,
		indexUpload->GetResource(),
		0,
		indexUpload->GetByteSize());
	// Build camera
	UploadBuffer cbufferUpload(
		device.get(),
		CalculateConstantBufferByteSize(sizeof(XMFLOAT4X4)));
	constBuffer = std::make_unique<DefaultBuffer>(
		device.get(),
		cbufferUpload.GetByteSize());

	mainCamera = std::make_unique<Camera>();
	mainCamera->Right = Math::Vector3(0.6877694, -1.622736E-05, 0.7259292);
	mainCamera->Up = Math::Vector3(-0.3181089, 0.8988663, 0.301407);
	mainCamera->Forward = Math::Vector3(-0.6525182, -0.438223, 0.6182076);
	mainCamera->Position = Math::Vector3(2.232773, 1.501817, -1.883978);
	mainCamera->SetAspect(static_cast<float>(m_scissorRect.right) / static_cast<float>(m_scissorRect.bottom));
	mainCamera->UpdateViewMatrix();
	mainCamera->UpdateProjectionMatrix();
	Math::Matrix4 viewProjMatrix = mainCamera->Proj * mainCamera->View;
	cbufferUpload.CopyData(
		0, {reinterpret_cast<vbyte const*>(&viewProjMatrix), sizeof(Math::Matrix4)});
	commandList->CopyBufferRegion(
		constBuffer->GetResource(),
		0,
		cbufferUpload.GetResource(),
		0,
		cbufferUpload.GetByteSize());
	ThrowIfFailed(commandList->Close());
	// Execute CommandList
	ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
	m_commandQueue->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
	// Create an empty root signature.
	{
		CD3DX12_ROOT_PARAMETER cbufferParameter;
		cbufferParameter.InitAsConstantBufferView(0);//register(b0);
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(1, &cbufferParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
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
		// ZWrite on, ZTest less
		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.StencilEnable = false;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
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
	// Execute and Present
	frameResources[curFrame]->Execute(
		m_commandQueue.Get(),
		m_fence.Get(),
		m_fenceValue);
	// Present the frame.
	ThrowIfFailed(m_swapChain->Present(0, 0));
	m_backBufferIndex = (m_backBufferIndex + 1) % FrameCount;
	// Signal Frame
	frameResources[curFrame]->Signal(
		m_commandQueue.Get(),
		m_fence.Get());
	// Populate next frame
	frameResources[nextFrame]->Sync(
		m_fence.Get());
	PopulateCommandList(frameResources[nextFrame]->Command(), nextFrame);
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
	cmdList->ResourceBarrier(1, get_rvalue_ptr(CD3DX12_RESOURCE_BARRIER::Transition(m_depthTarget.Get(), D3D12_RESOURCE_STATE_DEPTH_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), 0, m_dsvDescriptorSize);
	cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	// Record commands.
	const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
	cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferView;
	triangleMesh->GetVertexBufferView(vertexBufferView);
	//Set vertex & index buffer
	cmdList->IASetVertexBuffers(0, vertexBufferView.size(), vertexBufferView.data());
	D3D12_INDEX_BUFFER_VIEW indexBufferView = triangleMesh->GetIndexBufferView();
	cmdList->IASetIndexBuffer(&indexBufferView);
	cmdList->SetPipelineState(
		m_pipelineState.Get());

	cmdList->SetGraphicsRootConstantBufferView(
		0,
		constBuffer->GetAddress());
	cmdList->DrawIndexedInstanced(
		array_count(indices),
		1,
		0,
		0,
		0);

	// Indicate that the back buffer will now be used to present.
	cmdList->ResourceBarrier(1, get_rvalue_ptr(CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT)));
	cmdList->ResourceBarrier(1, get_rvalue_ptr(CD3DX12_RESOURCE_BARRIER::Transition(m_depthTarget.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ)));
}
D3D12SimpleBox::~D3D12SimpleBox() {}

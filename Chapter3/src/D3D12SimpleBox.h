//*********************************************************
//0
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "DXSample.h"
#include <Resource/Mesh.h>
#include <DXRuntime/Device.h>
#include <Resource/UploadBuffer.h>

using namespace DirectX;
// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;
class FrameResource;
class Camera;
class CommandListHandle;
class DefaultBuffer;
struct Vertex : public rtti::Struct {
	rtti::Var<XMFLOAT3> position = "POSITION";
	rtti::Var<XMFLOAT4> color = "COLOR";
};
class D3D12SimpleBox : public DXSample {
public:
	D3D12SimpleBox(uint32_t width, uint32_t height, std::wstring name);
	D3D12SimpleBox(D3D12SimpleBox const&) = delete;
	D3D12SimpleBox(D3D12SimpleBox&&) = delete;
	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;
	~D3D12SimpleBox();

private:
	static const uint32_t FrameCount = 3;
	std::unique_ptr<Device> device;
	std::unique_ptr<Camera> mainCamera;
	// Pipeline objects.
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12Resource> m_depthTarget;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	uint32_t m_rtvDescriptorSize;
	uint32_t m_dsvDescriptorSize;
	std::unique_ptr<FrameResource> frameResources[FrameCount];

	// App resources.
	std::unique_ptr<Mesh> triangleMesh;
	std::unique_ptr<DefaultBuffer> constBuffer;
	// Synchronization objects.
	uint32_t m_backBufferIndex;
	ComPtr<ID3D12Fence> m_fence;
	uint64_t m_fenceValue;
	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList(CommandListHandle const& cmdListHandle, uint frameIndex) const;
};

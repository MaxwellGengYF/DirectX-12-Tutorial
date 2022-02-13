#pragma once
#include <DXRuntime/Device.h>
#include <DXRuntime/CommandListHandle.h>
using Microsoft::WRL::ComPtr;

class FrameResource {
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	std::vector<ComPtr<ID3D12Resource>> delayDisposeResources;
	uint64 lastFenceIndex = 0;
	bool populated = false;

public:
	CommandListHandle Command();
	FrameResource(Device* device);
	~FrameResource();
	void AddDelayDisposeResource(ComPtr<ID3D12Resource> const& ptr);
	void Execute(
		ID3D12CommandQueue* queue,
		ID3D12Fence* fence,
		uint64& fenceIndex);
	void Signal(
		ID3D12CommandQueue* queue,
		ID3D12Fence* fence);
	void Sync(
		ID3D12Fence* fence);
};

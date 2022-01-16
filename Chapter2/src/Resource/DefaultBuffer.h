#pragma once
#include <Resource/Buffer.h>

class DefaultBuffer final : public Buffer {
private:
	uint64 byteSize;
	D3D12_RESOURCE_STATES initState;
	ComPtr<ID3D12Resource> resource;

public:
	ID3D12Resource* GetResource() const override { return resource.Get(); }
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const override { return resource->GetGPUVirtualAddress(); }
	uint64 GetByteSize() const override { return byteSize; }
	DefaultBuffer(
		Device* device,
		uint64 byteSize,
		D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON);
	~DefaultBuffer();
	D3D12_RESOURCE_STATES GetInitState() const override {	
		return initState;
	}

	DefaultBuffer(DefaultBuffer&&) = default;
	DefaultBuffer(DefaultBuffer const&) = delete;
};

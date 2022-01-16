#pragma once
#include <Resource/Buffer.h>

namespace toolhub::directx {
class ReadbackBuffer final : public Buffer {
private:
	ComPtr<ID3D12Resource> resource;
	uint64 byteSize;

public:
	ID3D12Resource* GetResource() const override { return resource.Get(); }
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const override { return resource->GetGPUVirtualAddress(); }
	uint64 GetByteSize() const override { return byteSize; }
	ReadbackBuffer(
		Device* device,
		uint64 byteSize);
	~ReadbackBuffer();
	void CopyData(uint64 offset, std::span<vbyte> data) const;
	D3D12_RESOURCE_STATES GetInitState() const override {
		return D3D12_RESOURCE_STATE_COPY_DEST;
	}
	ReadbackBuffer(ReadbackBuffer&&) = default;
	ReadbackBuffer(ReadbackBuffer const&) = delete;
};
}// namespace toolhub::directx
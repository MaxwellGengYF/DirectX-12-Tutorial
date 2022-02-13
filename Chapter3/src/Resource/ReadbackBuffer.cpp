
#include <Resource/ReadbackBuffer.h>
#include <DXSampleHelper.h>
#include <DXRuntime/FrameResource.h>

ReadbackBuffer::ReadbackBuffer(
	Device* device,
	uint64 byteSize)
	: Buffer(device),
	  byteSize(byteSize) {
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);
	auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource)));
}
ReadbackBuffer::~ReadbackBuffer() {
}
void ReadbackBuffer::CopyData(
	uint64 offset,
	std::span<vbyte> data) const {
	void* mapPtr;
	D3D12_RANGE range;
	range.Begin = offset;
	range.End = std::min(byteSize, offset + data.size());
	ThrowIfFailed(resource->Map(0, &range, (void**)(&mapPtr)));
	memcpy(data.data(), reinterpret_cast<vbyte const*>(mapPtr) + offset, range.End - range.Begin);
	resource->Unmap(0, nullptr);
}

void ReadbackBuffer::DelayDispose(FrameResource* frameRes) const {
	frameRes->AddDelayDisposeResource(resource);
}
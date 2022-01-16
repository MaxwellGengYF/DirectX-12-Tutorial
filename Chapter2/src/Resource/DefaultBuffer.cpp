#pragma vengine_package vengine_directx
#include <Resource/DefaultBuffer.h>
#include <DXSampleHelper.h>

DefaultBuffer::DefaultBuffer(
	Device* device,
	uint64 byteSize,
	D3D12_RESOURCE_STATES initState)
	: Buffer(device),
	  initState(initState),
	  byteSize(byteSize) {
	auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto buffer = CD3DX12_RESOURCE_DESC::Buffer(byteSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	ThrowIfFailed(device->DxDevice()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		initState,
		nullptr,
		IID_PPV_ARGS(&resource)));
}
DefaultBuffer::~DefaultBuffer() {
}

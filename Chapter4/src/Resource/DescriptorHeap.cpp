#include <Resource/DescriptorHeap.h>

DescriptorHeap::DescriptorHeap(
	Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE Type,
	uint64 numDescriptors,
	bool bShaderVisible)
	: Resource(device),
	  allocatePool(numDescriptors),
	  numDescriptors(numDescriptors) {
	Desc.Type = Type;
	Desc.NumDescriptors = numDescriptors;
	Desc.Flags = (bShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	Desc.NodeMask = 0;
	{
		for (size_t i = 0; i < numDescriptors; ++i) {
			allocatePool[i] = i;
		}
	}
	ThrowIfFailed(device->DxDevice()->CreateDescriptorHeap(
		&Desc,
		IID_PPV_ARGS(&pDH)));
	hCPUHeapStart = pDH->GetCPUDescriptorHandleForHeapStart();
	hGPUHeapStart = pDH->GetGPUDescriptorHandleForHeapStart();
	HandleIncrementSize = device->DxDevice()->GetDescriptorHandleIncrementSize(Desc.Type);
}
DescriptorHeap::~DescriptorHeap() {
}
uint DescriptorHeap::AllocateIndex() {
	std::lock_guard lck(heapMtx);
#ifdef _DEBUG
	if (allocatePool.empty()) {
		throw "bindless allocator out or range!\n";
	}
#endif
	auto last = allocatePool.end() - 1;
	uint v = *last;
	allocatePool.erase(last);
	return v;
}
void DescriptorHeap::ReturnIndex(uint v) {
	std::lock_guard lck(heapMtx);
	allocatePool.emplace_back(v);
}
void DescriptorHeap::Reset() {
	allocatePool.resize(numDescriptors);
	for (size_t i = 0; i < numDescriptors; ++i) {
		allocatePool[i] = i;
	}
}
void DescriptorHeap::CreateUAV(ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc, uint64 index) {
	device->DxDevice()->CreateUnorderedAccessView(resource, nullptr, &pDesc, hCPU(index));
}
void DescriptorHeap::CreateSRV(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc, uint64 index) {
	device->DxDevice()->CreateShaderResourceView(resource, &pDesc, hCPU(index));
}

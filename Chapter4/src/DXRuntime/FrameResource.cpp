#include <DXRuntime/FrameResource.h>
#include <DXSampleHelper.h>
FrameResource::FrameResource(Device* device) {
	ThrowIfFailed(
		device
			->DxDevice()
			->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
	ThrowIfFailed(
		device
			->DxDevice()
			->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList)));
	ThrowIfFailed(
		cmdList->Close());
}
FrameResource::~FrameResource() {
}
void FrameResource::AddDelayDisposeResource(ComPtr<ID3D12Resource> const& ptr) {
	delayDisposeResources.push_back(ptr);
}
void FrameResource::Signal(
	ID3D12CommandQueue* queue,
	ID3D12Fence* fence) {
	if (!populated) return;
	queue->Signal(fence, lastFenceIndex);
}
void FrameResource::Execute(
	ID3D12CommandQueue* queue,
	ID3D12Fence* fence,
	uint64& fenceIndex) {
	if (!populated) return;
	lastFenceIndex = ++fenceIndex;
	ID3D12CommandList* ppCommandLists[] = {cmdList.Get()};
	queue->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
}
void FrameResource::Sync(
	ID3D12Fence* fence) {
	if (!populated || lastFenceIndex == 0) return;
	if (fence->GetCompletedValue() < lastFenceIndex) {
		LPCWSTR falseValue = 0;
		HANDLE eventHandle = CreateEventEx(nullptr, falseValue, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(fence->SetEventOnCompletion(lastFenceIndex, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	delayDisposeResources.clear();
}
CommandListHandle FrameResource::Command() {
	populated = true;
	return {cmdAllocator.Get(), cmdList.Get()};
}
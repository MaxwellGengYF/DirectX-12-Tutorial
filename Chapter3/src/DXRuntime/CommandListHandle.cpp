#include <DXRuntime/CommandListHandle.h>
#include <DXSampleHelper.h>
CommandListHandle::CommandListHandle(CommandListHandle&& v)
	: cmdList(v.cmdList) {
	v.cmdList = nullptr;
}
CommandListHandle::CommandListHandle(
	ID3D12CommandAllocator* allocator,
	ID3D12GraphicsCommandList* cmdList)
	: cmdList(cmdList) {
	ThrowIfFailed(allocator->Reset());
	ThrowIfFailed(cmdList->Reset(allocator, nullptr));
}
CommandListHandle::~CommandListHandle() {
	if (cmdList)
		cmdList->Close();
}

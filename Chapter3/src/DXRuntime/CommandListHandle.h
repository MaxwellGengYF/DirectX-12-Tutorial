#pragma once
#include <d3d12.h>
class CommandListHandle {
	ID3D12GraphicsCommandList* cmdList;

public:
	ID3D12GraphicsCommandList* CmdList() const { return cmdList; }
	CommandListHandle(CommandListHandle const&) = delete;
	CommandListHandle(CommandListHandle&&);
	CommandListHandle(
		ID3D12CommandAllocator* allocator,
		ID3D12GraphicsCommandList* cmdList);
	~CommandListHandle();
};
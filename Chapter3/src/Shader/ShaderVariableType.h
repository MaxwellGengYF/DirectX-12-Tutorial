#pragma once
#include <stdafx.h>
#include <Windows.h>
#include <d3dx12.h>
enum class ShaderVariableType : uint8_t {
	ConstantBuffer,
	SRVDescriptorHeap,
	UAVDescriptorHeap,
	CBVDescriptorHeap,
	StructuredBuffer,
	RWStructuredBuffer
};
struct ShaderVariable {
	std::string name;
	ShaderVariableType type;
	uint tableSize;
	uint registerPos;
	uint space;
	ShaderVariable() {}
	ShaderVariable(
		const std::string& name,
		ShaderVariableType type,
		uint tableSize,
		uint registerPos,
		uint space)
		: name(name),
		  type(type),	
		  tableSize(tableSize),
		  registerPos(registerPos),
		  space(space) {}
};
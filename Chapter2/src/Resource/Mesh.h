#pragma once
#include <Resource/Resource.h>
#include <Resource/DefaultBuffer.h>
#include <Utility/ReflactableStruct.h>
class Mesh : public Resource {
	std::vector<DefaultBuffer> vertexBuffers;
	std::vector<rtti::Struct> vertexStructs;
	std::vector<D3D12_INPUT_ELEMENT_DESC> layout;
	uint64 vertexCount;

public:
	std::span<DefaultBuffer const> VertexBuffers() const { return vertexBuffers; }
	std::span<D3D12_INPUT_ELEMENT_DESC const> Layout() const { return layout; }
	Mesh(
		Device* device,
		std::vector<rtti::Struct>&& vbStructs,
		uint64 vertexCount);
	void GetVertexBufferView(std::vector<D3D12_VERTEX_BUFFER_VIEW>& result) const;
};
#pragma once
#include <Resource/Resource.h>
#include <Resource/DefaultBuffer.h>
#include <Utility/ReflactableStruct.h>
class Mesh : public Resource {
	std::vector<DefaultBuffer> vertexBuffers;
	DefaultBuffer indexBuffer;
	std::span<rtti::Struct const*> vertexStructs;
	std::vector<D3D12_INPUT_ELEMENT_DESC> layout;
	uint vertexCount;
	uint indexCount;

public:
	std::span<DefaultBuffer const> VertexBuffers() const { return vertexBuffers; }
	DefaultBuffer const& IndexBuffer() const { return indexBuffer; }
	std::span<D3D12_INPUT_ELEMENT_DESC const> Layout() const { return layout; }
	Mesh(
		Device* device,
		std::span<rtti::Struct const*> vbStructs,
		uint vertexCount,
		uint indexCount);
	void GetVertexBufferView(std::vector<D3D12_VERTEX_BUFFER_VIEW>& result) const;
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
};
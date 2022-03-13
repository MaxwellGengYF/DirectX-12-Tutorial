#pragma once
#include <stdafx.h>

class Buffer;
struct BufferView {
	Buffer const* buffer = nullptr;
	uint64 offset = 0;
	uint64 byteSize = 0;
	BufferView() {}
	BufferView(Buffer const* buffer);
	BufferView(
		Buffer const* buffer,
		uint64 offset,
		uint64 byteSize);
	bool operator==(BufferView const& a) const {
		return memcmp(this, &a, sizeof(BufferView)) == 0;
	}
	bool operator!=(BufferView const& a) const {
		return !operator==(a);
	}
};

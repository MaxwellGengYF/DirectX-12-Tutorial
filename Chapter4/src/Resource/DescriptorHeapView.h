#pragma once
#include <stdafx.h>
class DescriptorHeap;
struct DescriptorHeapView {
	DescriptorHeap const* heap;
	uint64 index;
	DescriptorHeapView(
		DescriptorHeap const* heap,
		uint64 index)
		: heap(heap),
		  index(index) {}
	DescriptorHeapView(
		DescriptorHeap const* heap)
		: heap(heap),
		  index(0) {}
};

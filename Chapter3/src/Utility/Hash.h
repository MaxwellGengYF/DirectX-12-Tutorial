#pragma once
#include <stdint.h>
#include <type_traits>
namespace xxh {
size_t xxhash_gethash(void const* ptr, size_t sz);
//Size must less than 32 in x64
size_t xxhash_gethash_small(void const* ptr, size_t sz);
}// namespace xxh
template<typename T>
requires(std::is_trivial_v<T> && (!std::is_reference_v<T>))
	size_t GetHash(T const& v) {
	if constexpr (sizeof(T) < 32) {
		return xxh::xxhash_gethash_small(&v, sizeof(T));
	} else {
		return xxh::xxhash_gethash(&v, sizeof(T));
	}
}

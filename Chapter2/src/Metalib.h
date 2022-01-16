#pragma once
#include <type_traits>
template<typename T>
requires(!std::is_lvalue_reference_v<T>)
	T* get_rvalue_ptr(T&& v) {
	return &v;
}
template<typename T>
struct array_meta;
template<typename T, size_t N>
struct array_meta<T[N]> {
	static constexpr size_t array_size = N;
	static constexpr size_t byte_size = N * sizeof(T);
};

template<typename T>
requires(std::is_bounded_array_v<T>) constexpr size_t array_count(T const& t) {
	return array_meta<T>::array_size;
}
template<typename T>
requires(std::is_bounded_array_v<T>) constexpr size_t array_byte_size(T const& t) {
	return array_meta<T>::byte_size;
}

using vbyte = uint8_t;
using uint = uint32_t;
using uint64 = uint64_t;
using int32 = int32_t;

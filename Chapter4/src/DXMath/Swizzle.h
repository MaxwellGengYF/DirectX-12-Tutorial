#pragma once
#include <stdint.h>
#include <type_traits>
template<bool enable_assignment, typename base_type, typename target_type, uint32_t... sequence>
struct Swizzle;

template<typename base_type, typename target_type, uint32_t... sequence>
struct Swizzle<true, base_type, target_type, sequence...> {
	inline operator target_type() const {
		const base_type* self = reinterpret_cast<const base_type*>(this);
		base_type pad[sizeof...(sequence)] = {self[sequence]...};
		return *reinterpret_cast<target_type*>(pad);
	}

	inline target_type operator=(target_type rhs) {
		base_type* self = reinterpret_cast<base_type*>(this);
		base_type* prhs = reinterpret_cast<base_type*>(&rhs);

		assign(self, prhs, std::make_index_sequence<sizeof...(sequence)>());

		return *reinterpret_cast<target_type*>(this);
	}

private:
	template<size_t... indices>
	inline void assign(base_type* self, base_type* rhs, std::index_sequence<indices...> seq) {
		base_type tmp[] = {self[sequence] = rhs[indices]...};
	}
};
template<typename base_type, typename target_type, uint32_t... sequence>
struct Swizzle<false, base_type, target_type, sequence...> {
	inline operator target_type() const {
		const base_type* self = reinterpret_cast<const base_type*>(this);
		base_type pad[sizeof...(sequence)] = {self[sequence]...};
		return *reinterpret_cast<target_type*>(pad);
	}

private:
	template<size_t... indices>
	inline void assign(base_type* self, base_type* rhs, std::index_sequence<indices...> seq) {
		base_type tmp[] = {self[sequence] = rhs[indices]...};
	}
};
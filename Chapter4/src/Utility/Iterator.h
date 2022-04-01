#pragma once
#include <stdint.h>
template<typename T>
class Iterator {
public:
	virtual ~Iterator() = default;
	virtual T operator*() = 0;
	virtual operator bool() = 0;
	virtual void operator++() = 0;
	void operator++(int32_t) { operator++(); }
};
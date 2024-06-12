#pragma once

#include "Defines.hpp"

class Allocator
{
public:
	virtual inline void* Allocate(U64 size) = 0;
	virtual inline void Free(void* ptr) = 0;
};

class DefaultAllocator : Allocator
{
public:
	inline virtual void* Allocate(U64 size) final { return malloc(size); }
	inline virtual void Free(void* ptr) final { return free(ptr); }
};
#pragma once

#include <stdlib.h>
#include <string.h>

typedef unsigned long long U64;
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

typedef signed long long I64;
typedef signed int I32;
typedef signed short I16;
typedef signed char I8;

typedef float F32;
typedef double F64;

#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

#define ASSERT(expr)		\
{							\
	if (expr) {}			\
	else { debugBreak(); }	\
}

template<typename T>
constexpr T&& Move(T&& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& Move(T& t) noexcept
{
    return static_cast<T&&>(t);
}
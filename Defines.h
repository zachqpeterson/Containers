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
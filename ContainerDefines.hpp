#pragma once

typedef unsigned char U8;		//Unsigned 8-bit integer
typedef unsigned short U16;		//Unsigned 16-bit integer
typedef unsigned int U32;		//Unsigned 32-bit integer
typedef unsigned long UL32;		//Unsigned 32-bit integer
typedef unsigned long long U64;	//Unsigned 64-bit integer

typedef signed char I8;			//Signed 8-bit integer
typedef signed short I16;		//Signed 16-bit integer
typedef signed int I32;			//Signed 32-bit integer
typedef signed long L32;		//Signed 32-bit integer
typedef signed long long I64;	//Signed 64-bit integer

typedef float F32;				//32-bit floating point number
typedef double F64;				//64-bit floating point number

typedef char CH;				//8-bit ascii character
typedef char8_t C8;				//8-bit ascii character
typedef char16_t C16;			//16-bit unicode character
typedef char32_t C32;			//32-bit unicode character
typedef wchar_t CW;				//Platform defined wide character, WINDOWS: 16-bit, OTHER: 32-bit

typedef decltype(__nullptr) NullPointer; //Nullptr type

static inline constexpr U64 U64_MAX = 0xFFFFFFFFFFFFFFFFULL;	//Maximum value of an unsigned 64-bit integer
static inline constexpr U64 U64_MIN = 0x0000000000000000ULL;	//Minimum value of an unsigned 64-bit integer
static inline constexpr I64 I64_MAX = 0x7FFFFFFFFFFFFFFFLL;		//Maximum value of a signed 64-bit integer
static inline constexpr I64 I64_MIN = 0x8000000000000000LL;		//Minimum value of a signed 64-bit integer
static inline constexpr U32 U32_MAX = 0xFFFFFFFFU;				//Maximum value of an unsigned 32-bit integer
static inline constexpr U32 U32_MIN = 0x00000000U;				//Minimum value of an unsigned 32-bit integer
static inline constexpr I32 I32_MAX = 0x7FFFFFFFI32;			//Maximum value of a signed 32-bit integer
static inline constexpr I32 I32_MIN = 0x80000000I32;			//Minimum value of a signed 32-bit integer
static inline constexpr UL32 UL32_MAX = 0xFFFFFFFFUL;			//Maximum value of an unsigned 32-bit integer
static inline constexpr UL32 UL32_MIN = 0x00000000UL;			//Minimum value of an unsigned 32-bit integer
static inline constexpr L32 L32_MAX = 0x7FFFFFFFL;				//Maximum value of a signed 32-bit integer
static inline constexpr L32 L32_MIN = 0x80000000L;				//Minimum value of a signed 32-bit integer
static inline constexpr U16 U16_MAX = 0xFFFFUI16;				//Maximum value of an unsigned 16-bit integer
static inline constexpr U16 U16_MIN = 0x0000UI16;				//Minimum value of an unsigned 16-bit integer
static inline constexpr I16 I16_MAX = 0x7FFFI16;				//Maximum value of a signed 16-bit integer
static inline constexpr I16 I16_MIN = 0x8000I16;				//Minimum value of a signed 16-bit integer
static inline constexpr U8 U8_MAX = 0xFFUI8;					//Maximum value of an unsigned 8-bit integer
static inline constexpr U8 U8_MIN = 0x00UI8;					//Minimum value of an unsigned 8-bit integer
static inline constexpr I8 I8_MAX = 0x7FI8;						//Maximum value of a signed 8-bit integer
static inline constexpr I8 I8_MIN = 0x80I8;						//Minimum value of a signed 8-bit integer
static inline constexpr F32 F32_MAX = 3.402823466e+38F;			//Maximum value of a 32-bit float
static inline constexpr F32 F32_MIN = 1.175494351e-38F;			//Minimum value of a 32-bit float
static inline constexpr F64 F64_MAX = 1.7976931348623158e+308;	//Maximum value of a 64-bit float
static inline constexpr F64 F64_MIN = 2.2250738585072014e-308;	//Minimum value of a 64-bit float
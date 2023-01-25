#pragma once

template<typename T, T... Args>
struct Initializer
{
	//TODO:
	constexpr Initializer() {}

	const U64 size = sizeof...(Args);
	const T list[sizeof...(Args)] = { Args... };
};
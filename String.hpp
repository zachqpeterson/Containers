#pragma once

#include "Defines.hpp"
#include "TypeTraits.hpp"

#include <string>

template<Character C> struct StringBase;

using String = StringBase<C8>;
using String8 = StringBase<C8>;
using String16 = StringBase<C16>;
using String32 = StringBase<C32>;
using WString = StringBase<CW>;

template<Character C>
struct StringBase
{
	StringBase();
	StringBase(const StringBase& other);
	StringBase(StringBase&& other) noexcept;
	StringBase(C* other);
	template<U64 Count> StringBase(const char(&other)[Count]);

	~StringBase();
	void Destroy();
	void Clear();

	StringBase& operator=(const StringBase& other);
	StringBase& operator=(StringBase&& other) noexcept;
	StringBase& operator=(C* other);
	template<U64 Count> StringBase& operator=(const C(&other)[Count]);

	StringBase&& SubString(U64 start, U64 length = U64_MAX);
	StringBase&& Appended(const String& append) const;
	StringBase&& Prepended(const String& prepend) const;
	StringBase&& Surrounded(const String& prepend, const String& append) const;

	bool Blank() const;
	I64 IndexOf(C c, U64 start = 0) const;
	I64 LastIndexOf(C c, U64 start = 0) const;

	StringBase& Trim();
	StringBase& ToUpper();
	StringBase& ToLower();

	const U64& Size() const;
	const U64& Capacity() const;

	C* Data();
	const C* Data() const;
	operator C* ();
	operator const C* () const;

	C* begin();
	C* end();
	const C* begin() const;
	const C* end() const;

	C* rbegin();
	C* rend();
	const C* rbegin() const;
	const C* rend() const;

private:

	U64 size{ 0 };
	U64 capacity{ 0 };
	C* string{ nullptr };
};

template<Character C>
inline StringBase<C>::StringBase() {}

template<Character C>
inline StringBase<C>::StringBase(const StringBase& other) : size{ other.size }, capacity{ other.capacity }, string{ (C*)calloc(capacity, 1) }
{
	memcpy(string, other.string, size);
}

template<Character C>
inline StringBase<C>::StringBase(StringBase&& other) noexcept : size{ other.size }, capacity{ other.capacity }, string{ other.string } {}

template<Character C>
inline StringBase<C>::StringBase(C* other) : size{ strlen(other) }, capacity{ size }, string{ (C*)calloc(capacity, 1) }
{
	memcpy(string, other, size);
}

template<Character C>
template<U64 Count>
inline StringBase<C>::StringBase(const char(&other)[Count]) : size{ Count }, capacity{ Count }, string{ (C*)calloc(1024, 1) }
{
	memcpy(string, other, Count);
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(const StringBase& other)
{
	size = other.size;
	capacity = other.capacity;
	string = calloc(capacity, 1);
	memcpy(string, other.string, size);

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(StringBase&& other) noexcept
{
	size = other.size;
	capacity = other.capacity;
	string = other.string;

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(C* other)
{
	size = strlen(other);
	capacity = size;
	string = calloc(capacity, 1);
	memcpy(string, other, size);

	return *this;
}

template<Character C>
template<U64 Count>
inline StringBase<C>& StringBase<C>::operator=(const C(&other)[Count])
{
	size = Count;
	capacity = Count;
	string = calloc(1024, 1);
	memcpy(string, other, Count);

	return *this;
}

template<Character C>
inline StringBase<C>::~StringBase()
{
	Destroy();
}

template<Character C>
inline void StringBase<C>::Destroy()
{
	if(string)
	{
		size = 0;
		capacity = 0;
		free(string);
		string = nullptr;
	}
}

template<Character C>
inline void StringBase<C>::Clear()
{
	size = 0;
}

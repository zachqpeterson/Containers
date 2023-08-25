#pragma once

#include "Defines.hpp"
#include "TypeTraits.hpp"

template<Character C> struct StringBase;

using String = StringBase<char>;
using String8 = StringBase<char8_t>;
using String16 = StringBase<char16_t>;
using String32 = StringBase<char32_t>;
using StringW = StringBase<wchar_t>;

template <class Type> inline constexpr bool IsStringType = AnyOf<RemovedQuals<Type>, String, String8, String16, String32, StringW>;
template <class Type> concept StringType = IsStringType<Type>;
template <class Type> inline constexpr bool IsNonStringPointer = IsPointer<Type> && !IsStringLiteral<Type>;
template <class Type> concept NonStringPointer = IsNonStringPointer<Type>;
template <class Type> inline constexpr bool IsNonStringClass = IsClass<Type> && !IsStringType<Type>;
template <class Type> concept NonStringClass = IsNonStringClass<Type>;

template<Character C>
void Copy(C* dst, const C* src, U64 length)
{
	constexpr U64 size = sizeof(C);
	memcpy(dst, src, length * size);
}

//TODO:
//Formatting
//
template<Character C>
struct StringBase
{
	StringBase();
	StringBase(NullPointer);
	StringBase(U64 length);
	StringBase(const C* other);
	StringBase(const C* other, U64 length);
	StringBase(const StringBase& other);
	StringBase(StringBase&& other) noexcept;

	StringBase& operator=(NullPointer);
	StringBase& operator=(const C* other);
	StringBase& operator=(const StringBase& other);
	StringBase& operator=(StringBase&& other) noexcept;

	~StringBase();
	void Destroy();
	void Clear();
	void Resize();
	void Reserve(U64 capacity);

	StringBase SubString(U64 start, U64 length = U64_MAX) const;
	StringBase Appended(const StringBase& append) const;
	StringBase Prepended(const StringBase& prepend) const;
	StringBase Surrounded(const StringBase& prepend, const StringBase& append) const;

	StringBase& Shave(U64 start, U64 length = U64_MAX);
	StringBase& Append(const StringBase& append);
	StringBase& Prepend(const StringBase& prepend);
	StringBase& Surround(const StringBase& prepend, const StringBase& append);

	bool Blank() const;
	I64 IndexOf(C c, U64 start = 0) const;
	I64 LastIndexOf(C c, U64 start = 0) const;

	StringBase& Trim();
	StringBase& ToUpper();
	StringBase& ToLower();
	StringBase& ToCapital();

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
	void Allocate(U64 length);
	void Reallocate(U64 length);
	constexpr U64 Length(const C* str) const;
	bool Blank(C c) const;
	bool NotBlank(C c) const;

	U64 size{ 0 };
	U64 capacity{ 0 };
	C* string{ nullptr };
};

template<Character C>
inline StringBase<C>::StringBase() {}

template<Character C>
inline StringBase<C>::StringBase(NullPointer) {}

template<Character C>
inline StringBase<C>::StringBase(U64 length) : size{ length }
{
	Allocate(size);
}

template<Character C>
inline StringBase<C>::StringBase(const C* other) : size{ Length(other) }
{
	Allocate(size);
	Copy(string, other, size);
	string[size] = '\0';
}

template<Character C>
inline StringBase<C>::StringBase(const C* other, U64 length) : size{ length }
{
	Allocate(size);
	Copy(string, other, size);
	string[size] = '\0';
}

template<Character C>
inline StringBase<C>::StringBase(const StringBase& other) : size{ other.size }
{
	Allocate(size);
	Copy(string, other.string, size + 1);
}

template<Character C>
inline StringBase<C>::StringBase(StringBase&& other) noexcept : size{ other.size }, capacity{ other.capacity }, string{ other.string }
{
	other.size = 0;
	other.capacity = 0;
	other.string = nullptr;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(NullPointer)
{
	Destroy();

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(const C* other)
{
	size = Length(other);

	Reallocate(size);
	Copy(string, other, size);
	string[size] = '\0';

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(const StringBase& other)
{
	if (&other == this) { return *this; }

	size = other.size;

	Reallocate(size);
	Copy(string, other.string, size + 1);

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::operator=(StringBase&& other) noexcept
{
	if (&other == this) { return *this; }

	Destroy();

	size = other.size;
	capacity = other.capacity;
	string = other.string;

	other.size = 0;
	other.capacity = 0;
	other.string = nullptr;

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
	if (string)
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
	string[0] = '\0';
	size = 0;
}

template<Character C>
inline void StringBase<C>::Resize()
{
	size = Length(string);
}

template<Character C>
inline void StringBase<C>::Reserve(U64 capacity)
{
	Reallocate(capacity);
}

template<Character C>
inline StringBase<C> StringBase<C>::SubString(U64 start, U64 length) const
{
	if (length != U64_MAX)
	{
		StringBase<C> str(string + start, length);
		str.string[length] = '\0';
		return Move(str);
	}
	else
	{
		StringBase<C> str(string + start, size - start);
		return Move(str);
	}
}

template<Character C>
inline StringBase<C> StringBase<C>::Appended(const StringBase<C>& append) const
{
	StringBase<C> str(size + append.Size());
	Copy(str.Data(), string, size);
	Copy(str.Data() + size, append.Data(), append.Size());
	str.Data()[str.Size()] = '\0';

	return Move(str);
}

template<Character C>
inline StringBase<C> StringBase<C>::Prepended(const StringBase<C>& prepend) const
{
	StringBase<C> str(size + prepend.Size());
	Copy(str.Data(), prepend.Data(), prepend.Size());
	Copy(str.Data() + prepend.Size(), string, size);
	str.Data()[str.Size()] = '\0';

	return Move(str);
}

template<Character C>
inline StringBase<C> StringBase<C>::Surrounded(const StringBase<C>& prepend, const StringBase<C>& append) const
{
	StringBase<C> str(size + prepend.Size() + append.Size());
	Copy(str.Data(), prepend.Data(), prepend.Size());
	Copy(str.Data() + prepend.Size(), string, size);
	Copy(str.Data() + prepend.Size() + size, append.Data(), append.Size());
	str.Data()[str.Size()] = '\0';

	return Move(str);
}

template<Character C>
inline StringBase<C>& StringBase<C>::Shave(U64 start, U64 length)
{
	if (length != U64_MAX)
	{
		Copy(string, string + start, length);
		size = length;
		string[length] = '\0';
		return *this;
	}
	else
	{
		Copy(string, string + start, size - start + 1);
		size -= start + 1;
		return *this;
	}
}

template<Character C>
inline StringBase<C>& StringBase<C>::Append(const StringBase<C>& append)
{
	if (size + append.Size() >= capacity) { Reallocate(size + append.Size() + 1); }

	Copy(string + size, append.Data(), append.Size());
	size += append.Size();
	string[size] = '\0';

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::Prepend(const StringBase<C>& prepend)
{
	if (size + prepend.Size() >= capacity) { Reallocate(size + prepend.Size() + 1); }

	Copy(string + prepend.Size(), string, size);
	Copy(string, prepend.Data(), prepend.Size());
	size += prepend.Size();
	string[size] = '\0';

	return *this;
}

template<Character C>
inline StringBase<C>& StringBase<C>::Surround(const StringBase<C>& prepend, const StringBase<C>& append)
{
	if (size + prepend.Size() + append.Size() >= capacity) { Reallocate(size + prepend.Size() + append.Size() + 1); }

	Copy(string + prepend.Size(), string, size);
	Copy(string, prepend.Data(), prepend.Size());
	Copy(string + prepend.Size() + size, append.Data(), append.Size());
	size += prepend.Size() + append.Size();
	string[size] = '\0';

	return *this;
}

template<Character C>
inline bool StringBase<C>::Blank() const
{
	if (!string) { return true; }

	C* it = string;
	C c = '\0';

	while ((c = *it++) != '\0') { if (NotBlank(c)) { return false; } }

	return true;
}

template<Character C>
inline I64 StringBase<C>::IndexOf(C ch, U64 start) const
{
	if (start >= size) { return -1; }

	C* it = string + start;
	C* end = string + size;
	C c = '\0';

	while ((c = *it) != ch && it != end) { ++it; }

	if (c == ch) { return it - string; }

	return -1;
}

template<Character C>
inline I64 StringBase<C>::LastIndexOf(C ch, U64 start) const
{
	if (start >= size) { return -1; }

	C* it = string + size - start - 2;
	C c = '\0';

	while ((c = *it) != ch && it != string) { --it; }

	if (c == ch) { return it - string; }

	return -1;
}

template<Character C>
inline const U64& StringBase<C>::Size() const
{
	return size;
}

template<Character C>
inline const U64& StringBase<C>::Capacity() const
{
	return capacity;
}

template<Character C>
inline C* StringBase<C>::Data()
{
	return string;
}

template<Character C>
inline const C* StringBase<C>::Data() const
{
	return string;
}

template<Character C>
inline StringBase<C>::operator C* ()
{
	return string;
}

template<Character C>
inline StringBase<C>::operator const C* () const
{
	return string;
}

template<Character C>
inline C* StringBase<C>::begin()
{
	return string;
}

template<Character C>
inline C* StringBase<C>::end()
{
	return string + size;
}

template<Character C>
inline const C* StringBase<C>::begin() const
{
	return string;
}

template<Character C>
inline const C* StringBase<C>::end() const
{
	return string + size;
}

template<Character C>
inline C* StringBase<C>::rbegin()
{
	return string + size - 1;
}

template<Character C>
inline C* StringBase<C>::rend()
{
	return string - 1;
}

template<Character C>
inline const C* StringBase<C>::rbegin() const
{
	return string + size - 1;
}

template<Character C>
inline const C* StringBase<C>::rend() const
{
	return string - 1;
}



//Helpers
template<Character C>
inline void StringBase<C>::Allocate(U64 length)
{
	constexpr U64 size = sizeof(C);

	capacity = length < 1024 ? 1024 : BitCeiling(length);

	string = (C*)malloc(capacity * size);
}

template<Character C>
inline void StringBase<C>::Reallocate(U64 length)
{
	constexpr U64 size = sizeof(C);

	if (length <= capacity) { return; }

	capacity = length < 1024 ? 1024 : BitCeiling(length);

	if (string) { string = (C*)realloc(string, capacity * size); }
	else { string = (C*)malloc(capacity * size); }
}

template<Character C>
constexpr inline U64 StringBase<C>::Length(const C* str) const
{
	const C* it = str;
	while (*it) { ++it; }

	return it - str;
}

template<Character C>
inline bool StringBase<C>::Blank(C c) const
{
	if constexpr (IsSame<C, char>)
	{
		return c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f' || c == ' ';
	}
	else if constexpr (IsSame<C, char8_t>)
	{
		return c == u8'\n' || c == u8'\r' || c == u8'\t' || c == u8'\v' || c == u8'\f' || c == u8' ';
	}
	else if constexpr (IsSame<C, char16_t>)
	{
		return c == u'\n' || c == u'\r' || c == u'\t' || c == u'\v' || c == u'\f' || c == u' ';
	}
	else if constexpr (IsSame<C, char32_t>)
	{
		return c == U'\n' || c == U'\r' || c == U'\t' || c == U'\v' || c == U'\f' || c == U' ';
	}
	else if constexpr (IsSame<C, wchar_t>)
	{
		return c == L'\n' || c == L'\r' || c == L'\t' || c == L'\v' || c == L'\f' || c == L' ';
	}
}

template<Character C>
inline bool StringBase<C>::NotBlank(C c) const
{
	if constexpr (IsSame<C, char>)
	{
		return c != '\n' && c != '\r' && c != '\t' && c != '\v' && c != '\f' && c != ' ';
	}
	else if constexpr (IsSame<C, char8_t>)
	{
		return c != u8'\n' && c != u8'\r' && c != u8'\t' && c != u8'\v' && c != u8'\f' && c != u8' ';
	}
	else if constexpr (IsSame<C, char16_t>)
	{
		return c != u'\n' && c != u'\r' && c != u'\t' && c != u'\v' && c != u'\f' && c != u' ';
	}
	else if constexpr (IsSame<C, char32_t>)
	{
		return c != U'\n' && c != U'\r' && c != U'\t' && c != U'\v' && c != U'\f' && c != U' ';
	}
	else if constexpr (IsSame<C, wchar_t>)
	{
		return c != L'\n' && c != L'\r' && c != L'\t' && c != L'\v' && c != L'\f' && c != L' ';
	}
}
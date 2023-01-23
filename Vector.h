#pragma once

#include "Defines.h"
#include "Allocator.h"
#include "Initiazer.h"

template<typename T>
struct Vector
{
public:
	Vector();
	Vector(U64 capacity);
	Vector(U64 size, const T& value);
	Vector(const Vector& other);
	Vector(Vector&& other) noexcept;
	template<T... Args> constexpr Vector(const Initializer<T, Args...>& init);

	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other) noexcept;
	template<T... Args> constexpr Vector& operator=(const Initializer<T, Args...>& init);

	~Vector();
	inline void Destroy();
	void* operator new(U64 size);
	void operator delete(void* ptr);

	void Push(const T& value);
	void Push(T&& value) noexcept;
	void Pop();
	void Pop(T& value);
	void Pop(T&& value) noexcept;
	void Insert(U64 index, const T& value);
	void Insert(U64 index, T&& value) noexcept;
	void Insert(U64 index, const Vector& other);
	void Insert(U64 index, Vector&& other) noexcept;
	void Remove(U64 index);

	void Split(U64 index, Vector& other);
	void Merge(const Vector& other);
	void Merge(Vector&& other) noexcept;
	Vector& operator+=(const Vector& other);
	Vector& operator+=(Vector&& other) noexcept;

	void Reserve(U64 capacity);
	void Resize(U64 size, const T& value);
	void Resize(U64 size);
	void Clear() { size = 0; }

	bool Contains(const T& value) const;
	U64 HowMany(const T& value) const;
	U64 Find(const T& value) const;

	const U64& Size() const { return size; }
	const U64& Capacity() const { return capacity; }
	const T* Data() const { return array; }
	T* Data() { return array; }

	const T& operator[](U64 i) const { return array[i]; }
	T& operator[](U64 i) { return array[i]; }

	T& Front() { return *array; }
	const T& Front() const { return *array; }
	T& Back() { return array[size - 1]; }
	const T& Back() const { return array[size - 1]; }
	T* begin() { return array; }
	T* end() { return array + size; }
	const T* begin() const { return array; }
	const T* end() const { return array + size; }

private:
	U64 size;
	U64 capacity;
	T* array;
};

template<typename T> inline Vector<T>::Vector() : size{ 0 }, capacity{ 0 }, array{ nullptr } {}

template<typename T> inline Vector<T>::Vector(U64 capacity) : size{ 0 }, capacity{ capacity }, array{ (T*)malloc(sizeof(T) * capacity) } {}

template<typename T> inline Vector<T>::Vector(U64 size, const T& value) : size{ size }, capacity{ size }, array{ (T*)malloc(sizeof(T) * capacity) }
{
	for (T* t = array, *end = array + size; t != end; ++t) { *t = value; }
}

template<typename T> inline Vector<T>::Vector(const Vector<T>& other) : size{ other.size }, capacity{ other.capacity }, array{ (T*)malloc(sizeof(T) * capacity) }
{
	memcpy(array, other.array, sizeof(T)* size);
}

template<typename T> inline Vector<T>::Vector(Vector<T>&& other) noexcept : size{ other.size }, capacity{ other.capacity }, array{ other.array }
{
	other.array = nullptr;
	other.size = 0;
	other.capacity = 0;
}

template<typename T> template<T... Args> inline constexpr Vector<T>::Vector(const Initializer<T, Args...>& init) :
	size{ init.size }, capacity{ size }, array{ (T*)malloc(sizeof(T) * capacity) }
{
	memcpy(array, init.list, sizeof(T)* size);
}

template<typename T> inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (array) { free(array); }
	size = other.size;
	capacity = other.capacity;
	array = (T*)malloc(sizeof(T) * capacity);

	memcpy(array, other.array, sizeof(T) * size);

	return *this;
}

template<typename T> inline Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept
{
	if (array) { free(array); }
	size = other.size;
	capacity = other.capacity;
	array = other.array;

	other.array = nullptr;
	other.size = 0;
	other.capacity = 0;

	return *this;
}

template<typename T> template<T... Args> inline constexpr Vector<T>& Vector<T>::operator=(const Initializer<T, Args...>& init)
{
	free(array);
	size = init.size;
	capacity = size;
	array = (T*)malloc(sizeof(T) * capacity);
	memcpy(array, init.list, sizeof(T) * size);
}

template<typename T> inline Vector<T>::~Vector() { Destroy(); }

template<typename T> inline void Vector<T>::Destroy() { size = 0; capacity = 0; if (array) { free(array); } array = nullptr; }

template<typename T> inline void* Vector<T>::operator new(U64 size) { return malloc(size); }

template<typename T> inline void Vector<T>::operator delete(void* ptr) { free(ptr); }

template<typename T> inline void Vector<T>::Push(const T& value)
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	array[size] = value;
	++size;
}

template<typename T> inline void Vector<T>::Push(T&& value) noexcept
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	array[size] = Move(value);
	++size;
}

template<typename T> inline void Vector<T>::Pop(T& value)
{
	if (size) { value = array[--size]; }
}

template<typename T> inline void Vector<T>::Insert(U64 index, const T& value)
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	memcpy(array + index + 1, array + index, sizeof(T) * (size - index));
	array[index] = value;
	++size;
}

template<typename T> inline void Vector<T>::Insert(U64 index, T&& value) noexcept
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	memcpy(array + index + 1, array + index, sizeof(T) * (size - index));
	array[index] = Move(value);
	++size;
}

template<typename T> inline void Vector<T>::Insert(U64 index, const Vector<T>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + index + other.size, array + index, sizeof(T) * (size - index));
	memcpy(array + index, other.array, sizeof(T) * (other.size));
	size += other.size;
}

template<typename T> inline void Vector<T>::Insert(U64 index, Vector<T>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + index + other.size, array + index, sizeof(T) * (size - index));
	memcpy(array + index, other.array, sizeof(T) * (other.size));
	size += other.size;

	other.~Vector();
}

template<typename T> inline void Vector<T>::Remove(U64 index)
{
	memcpy(array + index, array + index + 1, sizeof(T) * (size - index));

	--size;
}

template<typename T> inline void Vector<T>::Split(U64 index, Vector<T>& other)
{
	other.Reserve(size - index + 1);
	other.size = other.capacity;

	memcpy(other.array, array + index - 1, sizeof(T) * (other.size));

	size -= index;
}

template<typename T> inline void Vector<T>::Merge(const Vector<T>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + size, other.array, sizeof(T) * (other.size));
	size += other.size;
}

template<typename T> inline void Vector<T>::Merge(Vector<T>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + size, other.array, sizeof(T) * (other.size));
	size += other.size;

	other.~Vector();
}

template<typename T> inline Vector<T>& Vector<T>::operator+=(const Vector<T>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + size, other.array, sizeof(T) * (other.size));
	size += other.size;

	return *this;
}

template<typename T> inline Vector<T>& Vector<T>::operator+=(Vector<T>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	memcpy(array + size, other.array, sizeof(T) * (other.size));
	size += other.size;

	other.~Vector();

	return *this;
}

template<typename T> inline void Vector<T>::Reserve(U64 capacity)
{
	this->capacity = capacity;

	T* temp = (T*)realloc(array, sizeof(T) * capacity);
	array = temp ? temp : (T*)malloc(capacity);
}

template<typename T> inline void Vector<T>::Resize(U64 size, const T& value)
{
	capacity = size;
	this->size = size;
	T* temp = (T*)realloc(array, sizeof(T) * capacity);
	array = temp ? temp : (T*)malloc(capacity);

	for (U64 i = 0; i < size; ++i) { array[i] = value; }
}

template<typename T> inline void Vector<T>::Resize(U64 size)
{
	capacity = size;
	this->size = size;
	T* temp = (T*)realloc(array, sizeof(T) * capacity);
	array = temp ? temp : (T*)malloc(capacity);
}

template<typename T> inline bool Vector<T>::Contains(const T& value) const
{
	for (T* t = array, *end = array + size; t != end; ++t)
	{
		if (*t == value) { return true; }
	}

	return false;
}

template<typename T> inline U64 Vector<T>::HowMany(const T& value) const
{
	U64 count = 0;
	for (T* t = array, *end = array + size; t != end; ++t)
	{
		if (*t == value) { ++count; }
	}

	return count;
}

template<typename T> inline U64 Vector<T>::Find(const T& value) const
{
	U64 index = 0;
	for (T* t = array; index < size; ++index, ++t)
	{
		if (*t == value) { return index; }
	}

	return -1;
}
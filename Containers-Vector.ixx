module;

#include "ContainerDefines.hpp"

#include <memory>
#include <initializer_list>
#include <type_traits>

export module Containers:Vector;

template <class Type> concept FunctionPtr = std::is_function_v<Type>;

//TODO: Allocate function that uses new or malloc based on if theres a constructor

template <class Type>
inline Type* MoveValue(Type* dst, Type&& value) noexcept
{
	if constexpr (std::is_move_constructible_v<Type>)
	{
		new (dst) Type(std::move(value));
	}
	else if constexpr (std::is_move_assignable_v<Type>)
	{
		*dst = std::move(value);
	}
	else
	{
		memcpy(dst, &value, sizeof(Type));
		if constexpr (std::is_destructible_v<Type>) { value.~Type(); }
	}

	return dst;
}

template <class Type>
inline Type* CopyValue(Type* dst, const Type& value) noexcept
{
	if constexpr (std::is_copy_constructible_v<Type>)
	{
		new (dst) Type(value);
	}
	else if constexpr (std::is_copy_assignable_v<Type>)
	{
		*dst = value;
	}
	else
	{
		memcpy(dst, &value, sizeof(Type));
	}

	return dst;
}

template <class Type>
Type* MoveValues(Type* dst, Type* src, U64 count)
{
	if (dst > src && dst < src + count) //Reverse Copy
	{
		Type* rDst = dst + count - 1;
		Type* rSrc = src + count - 1;

		//TODO: look into unrolling
		for (U64 i = 0; i < count; ++i)
		{
			MoveValue(rDst--, std::move(*rSrc--));
		}
	}
	else
	{
		for (U64 i = 0; i < count; ++i)
		{
			MoveValue(dst++, std::move(*src++));
		}
	}

	return dst;
}

template <class Type>
Type* CopyValues(Type* dst, Type* src, U64 count)
{
	if (dst > src && dst < src + count) //Reverse Copy
	{
		Type* rDst = dst + count - 1;
		Type* rSrc = src + count - 1;

		//TODO: look into unrolling
		for (U64 i = 0; i < count; ++i)
		{
			CopyValue(rDst--, *rSrc--);
		}
	}
	else
	{
		for (U64 i = 0; i < count; ++i)
		{
			CopyValue(dst++, *src++);
		}
	}

	return dst;
}

/*
* TODO: Emplace
* 
* TODO: Variatic Templates
*	Insert
*	Merge
*	Add
*
* TODO: Initializer
*	Insert
*	Merge
*	Add
*
* TODO: Sorting
*	Comparison Predicate
*
* TODO: Comparison
*	Compare two Vectors, return true if matching
*	Compare two Vectors, return count of matching
*	Compare two Vectors, return Vector<T> of matching
*	Compare two Vectors, remove matching, return count
*	Compare two Vectors, merge all unique values
*
* TODO: Insertion
*	Push value if unique
*	Insert value if unique
*
* TODO: Remove
*	Search, remove all non-unique values
*/
export template<class Type>
struct Vector
{
	using Predicate = bool(const Type&);
	using Compare = bool(const Type&, const Type&);

public:
	/// <summary>
	/// Creates a new Vector instance, size and capacity will be zero, array will be nullptr
	/// </summary>
	Vector();

	/// <summary>
	/// Creates a new Vector instance, size will be zero, creates an array of size greater than or equal to sizeof(T) * capacity
	/// </summary>
	/// <param name="capacity:">The capacity the array will be at</param>
	Vector(U64 capacity);

	/// <summary>
	/// Creates a new Vector instance, capacity will be greater than or equal to size, creates an array of size sizeof(T) * capacity and fills it with value
	/// </summary>
	/// <param name="size:">The size the array will be</param>
	/// <param name="value:">The value that the array will be filled with</param>
	Vector(U64 size, const Type& value);

	/// <summary>
	/// Create a new Vector instance using an initializer list, size will equal the list size, capacity with be greater than or equal to size, creates an array of size sizeof(T) * capacity and fills it with the values in list
	/// </summary>
	/// <param name="list:">The initializer list</param>
	Vector(std::initializer_list<Type> list);

	/// <summary>
	/// Creates a new Vector instance, capacity and size will be other's, creates an array of the same size and copies other's data into it
	/// </summary>
	/// <param name="other:">Vector to copy</param>
	Vector(const Vector& other);

	/// <summary>
	/// Creates a new Vector instance, capacity and size will be other's, sets the array to other's 
	/// WARNING: other will be destroyed
	/// </summary>
	/// <param name="other:">The Vector to move</param>
	Vector(Vector&& other) noexcept;

	/// <summary>
	/// Copies other's data into this, capacity and size will be other's, creates an array of the same size and copies other's data into it
	/// WARNING: any previous data will be lost
	/// </summary>
	/// <param name="other:">The Vector to copy</param>
	/// <returns>Reference to this</returns>
	Vector& operator=(const Vector& other);

	/// <summary>
	/// Moves other's data into this, capacity and size will be other's, sets the array to other's 
	/// WARNING: any previous data will be lost
	/// WARNING: other will be destroyed
	/// </summary>
	/// <param name="other:">The Vector to move</param>
	/// <returns>Reference to this</returns>
	Vector& operator=(Vector&& other) noexcept;



	~Vector();

	/// <summary>
	/// Destroys data inside this, capacity and size will be zero, array will be nullptr
	/// </summary>
	void Destroy();



	/// <summary>
	/// Increases size by one and puts value onto the back of array, reallocates array if it's too small
	/// </summary>
	/// <param name="value:">The value to put into array</param>
	Type& Push(const Type& value);

	/// <summary>
	/// Increases size by one and moves value onto the back of array, reallocates array if it's too small
	/// </summary>
	/// <param name="value:">The value to move into array</param>
	Type& Push(Type&& value) noexcept;

	/// <summary>
	/// Decreases the size by one
	/// </summary>
	void Pop();

	/// <summary>
	/// Decreases the size by one and copies what was in the back of array to value
	/// </summary>
	/// <param name="value:">The value to copy to</param>
	void Pop(Type& value);

	/// <summary>
	/// Inserts value into index, moves values at and past index over, reallocates array if it's too small
	/// </summary>
	/// <param name="index:">The index to put value</param>
	/// <param name="value:">The value to copy</param>
	/// <returns>Reference to the inserted value</returns>
	Type& Insert(U64 index, const Type& value);

	/// <summary>
	/// Inserts value into index, moves values at and past index over, reallocates array if it's too small
	/// </summary>
	/// <param name="index:">The index to put value</param>
	/// <param name="value:">The value to move</param>
	/// <returns>Reference to the inserted value</returns>
	Type& Insert(U64 index, Type&& value) noexcept;

	/// <summary>
	/// Copies other and inserts it into index, moves values at and past index over, reallocates array if it's too small
	/// </summary>
	/// <param name="index:">The index to copy other into</param>
	/// <param name="other:">The Vector to copy</param>
	void Insert(U64 index, const Vector& other);

	/// <summary>
	/// Moves other and inserts it into index, moves values at and past index over, reallocates array if it's too small
	/// WARNING: other will be destroyed
	/// </summary>
	/// <param name="index:">The index to move other into</param>
	/// <param name="other:">The Vector to move</param>
	void Insert(U64 index, Vector&& other) noexcept;

	/// <summary>
	/// Moves values past index to index
	/// </summary>
	/// <param name="index:">The index to remove</param>
	void Remove(U64 index);

	/// <summary>
	/// Copies value at index into value, moves values past index to index
	/// </summary>
	/// <param name="index:">The index to remove</param>
	/// <param name="value:">The value to copy to</param>
	void Remove(U64 index, Type& value);

	/// <summary>
	/// Removes the value at index by swaping it with the last index
	/// </summary>
	/// <param name="index:">The index to remove</param>
	void RemoveSwap(U64 index);

	/// <summary>
	/// Copies value at index into value, removes the value at index by swaping it with the last index
	/// </summary>
	/// <param name="index:">The index to remove</param>
	/// <param name="value:">The value to copy to</param>
	void RemoveSwap(U64 index, Type& value);

	/// <summary>
	/// Moves values at and past index1 to index0
	/// </summary>
	/// <param name="index0:">The beginning of the erasure, inclusive</param>
	/// <param name="index1:">The end of the erasure, exclusive</param>
	void Erase(U64 index0, U64 index1);

	/// <summary>
	/// Copies values to be erased into other, moves values at and past index1 to index0
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="index0:">The beginning of the erasure, inclusive</param>
	/// <param name="index1:">The end of the erasure, exclusive</param>
	/// <param name="other:">The Vector to copy to</param>
	void Steal(U64 index0, U64 index1, Vector& other);



	/// <summary>
	/// Splits the array at index, copies data at and past index into other
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="index:">The index to split on, inclusive</param>
	/// <param name="other:">The Vector to copy to</param>
	void Split(U64 index, Vector& other);

	/// <summary>
	/// Copies data in other to the end of the array, reallocates the array if it's too small
	/// </summary>
	/// <param name="other:">The Vector to copy from</param>
	void Merge(const Vector& other);

	/// <summary>
	/// Moves data in other to the end of the array, reallocates the array if it's too small
	/// WARNING: other will be destroyed
	/// </summary>
	/// <param name="other:">The Vector to move</param>
	void Merge(Vector&& other) noexcept;

	/// <summary>
	/// Copies data in other to the end of the array, reallocates the array if it's too small
	/// </summary>
	/// <param name="other:">The Vector to copy from</param>
	/// <returns>Reference to this</returns>
	Vector& operator+=(const Vector& other);

	/// <summary>
	/// Moves data in other to the end of the array, reallocates the array if it's too small
	/// WARNING: other will be destroyed
	/// </summary>
	/// <param name="other:">The Vector to move</param>
	/// <returns>Reference to this</returns>
	Vector& operator+=(Vector&& other) noexcept;



	/// <summary>
	/// Searches array, finds all values that satisfy predicate, fill other with those values
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <param name="other:">A Vector to fill with values</param>
	void SearchFor(Predicate predicate, Vector& other);

	/// <summary>
	/// Searches array, finds indices of all values that satisfy predicate, fill other with those indices
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <param name="other:">A Vector to fill with indices</param>
	void SearchForIndices(Predicate predicate, Vector<U64>& other);

	/// <summary>
	/// Searches array, finds all values that satisfy predicate
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <returns>The count of values that satisfy predicate</returns>
	U64 SearchCount(Predicate predicate);

	/// <summary>
	/// Searches array, finds all values that satisfy predicate, removes them from array
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <returns>The count of values that satisfy predicate</returns>
	U64 RemoveAll(Predicate predicate);

	/// <summary>
	/// Searches array, finds all values that satisfy predicate, removes them from array and puts them into other
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <param name="other:">A Vector to fill with values</param>
	void RemoveAll(Predicate predicate, Vector& other);

	/// <summary>
	/// Searches array, finds all values that satisfy predicate, removes them from array by overriding with current last element
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <returns>The count of values that satisfy predicate</returns>
	U64 RemoveSwapAll(Predicate predicate);

	/// <summary>
	/// Searches array, finds all values that satisfy predicate, removes them from array by overriding with current last element and puts them into other
	/// WARNING: any previous data in other will be lost
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <param name="other:">A Vector to fill with values</param>
	void RemoveSwapAll(Predicate predicate, Vector& other);

	/// <summary>
	/// Finds the first value that satisfies the predicate, return true if one exists
	/// </summary>
	/// <param name="predicate:">A function to evaluate values: bool pred(const Type&amp; value)</param>
	/// <param name="value:">A reference to return the found value</param>
	/// <returns>true if a value exists, false otherwise</returns>
	Type* Find(Predicate predicate);



	/// <summary>
	/// Inserts a value based on a predicate
	/// </summary>
	/// <param name="predicate:">A function to compare values: bool pred(const Type&amp; a, const Type&amp; b)</param>
	/// <param name="value:">The value to insert</param>
	/// <returns>The index the value was inserted at</returns>
	U64 SortedInsert(Compare predicate, const Type& value);

	/// <summary>
	/// Inserts a value based on a predicate
	/// </summary>
	/// <param name="predicate:">A function to compare values: bool pred(const Type&amp; a, const Type&amp; b)</param>
	/// <param name="value:">The value to insert</param>
	/// <returns>The index the value was inserted at</returns>
	U64 SortedInsert(Compare predicate, Type&& value) noexcept;



	/// <summary>
	/// Reallocates the array to be sizeof(T) * capacity
	/// </summary>
	/// <param name="capacity:">The capacity the array will be at</param>
	void Reserve(U64 capacity);

	/// <summary>
	/// Sets size, reallocates the array if it's too small
	/// </summary>
	/// <param name="size:">The size to set to</param>
	void Resize(U64 size);

	/// <summary>
	/// Sets size, fills the array with value, reallocates the array if it's too small
	/// </summary>
	/// <param name="size:">The size to set to</param>
	/// <param name="value:">The value to fill the array with</param>
	void Resize(U64 size, const Type& value);

	/// <summary>
	/// Sets size to zero
	/// </summary>
	void Clear() { size = 0; }



	/// <summary>
	/// Searches array for value
	/// </summary>
	/// <param name="value:">The value to search for</param>
	/// <returns>true if value is contained within array, false otherwise</returns>
	bool Contains(const Type& value) const;

	/// <summary>
	/// Counts the reoccurrences of value in array
	/// </summary>
	/// <param name="value:">The value to search for</param>
	/// <returns>The number of reoccurrences of value</returns>
	U64 Count(const Type& value) const;

	/// <summary>
	/// Finds the first index of value
	/// </summary>
	/// <param name="value:">The value to search for</param>
	/// <returns>The index of value, if it doesn't find value, U64_MAX</returns>
	U64 Find(const Type& value) const;



	/// <returns>The current amount of elements</returns>
	U64 Size() const { return size; }

	/// <returns>The current maximum allowed elements</returns>
	U64 Capacity() const { return capacity; }

	/// <returns>Whether or not this is empty</returns>
	bool Empty() const { return size == 0; }

	/// <returns>Whether or not this is full</returns>
	bool Full() const { return size == capacity; }

	/// <summary></summary>
	/// <returns>array (const)</returns>
	const Type* Data() const { return array; }

	/// <summary></summary>
	/// <returns>array</returns>
	Type* Data() { return array; }



	/// <summary></summary>
	/// <param name="i:">Index</param>
	/// <returns>The value at index (const)</returns>
	const Type& operator[](U64 i) const { return array[i]; }

	/// <summary></summary>
	/// <param name="i:">Index</param>
	/// <returns>The value at index</returns>
	Type& operator[](U64 i) { return array[i]; }

	/// <summary>
	/// Gets the value at the front of array (index 0)
	/// </summary>
	/// <returns>The value at the front of array</returns>
	Type& Front() { return *array; }

	/// <summary>
	/// Gets the value at the front of array (index 0)
	/// </summary>
	/// <returns>The value at the front of array (const)</returns>
	const Type& Front() const { return *array; }

	/// <summary>
	/// Gets the value at the back of array (index size - 1)
	/// </summary>
	/// <returns>The value at the back of array</returns>
	Type& Back() { return array[size - 1]; }

	/// <summary>
	/// Gets the value at the back of array (index size - 1)
	/// </summary>
	/// <returns>The value at the back of array (const)</returns>
	const Type& Back() const { return array[size - 1]; }



	/// <summary>
	/// Compares the values stored in both vectors
	/// </summary>
	/// <param name="other: ">The other vector to compare against</param>
	/// <returns>True if the two vectors have the same values</returns>
	bool operator==(const Vector& other) const;

	/// <summary>
	/// Compares the values stored in both vectors
	/// </summary>
	/// <param name="other: ">The other vector to compare against</param>
	/// <returns>True if the two vectors have different values</returns>
	bool operator!=(const Vector& other) const;



	/// <summary></summary>
	/// <returns>The beginning of array as an iterator</returns>
	Type* begin() { return array; }

	/// <summary></summary>
	/// <returns>The end of array as an iterator</returns>
	Type* end() { return array + size; }

	/// <summary></summary>
	/// <returns>The beginning of array as an iterator (const)</returns>
	const Type* begin() const { return array; }

	/// <summary></summary>
	/// <returns>The end of array as an iterator (const)</returns>
	const Type* end() const { return array + size; }

private:
	U64 size{ 0 };
	U64 capacity{ 0 };
	Type* array{ nullptr };
};

template<class Type> 
inline Vector<Type>::Vector() {}

template<class Type> 
inline Vector<Type>::Vector(U64 capacity) : capacity(capacity), array((Type*)malloc(capacity * sizeof(Type))) {}

template<class Type> 
inline Vector<Type>::Vector(U64 size, const Type& value) : size(size), capacity(size), array((Type*)malloc(capacity * sizeof(Type)))
{
	for (Type* it = array, *end = array + size; it != end; ++it) { CopyValue(it, value); }
}

template <class Type>
using Initializer = std::initializer_list<Type>;

template<class Type> 
inline Vector<Type>::Vector(std::initializer_list<Type> list) : size(list.size()), capacity(size), array((Type*)malloc(capacity * sizeof(Type)))
{
	Type* it1 = array;
	for (const Type* it0 = list.begin(), *end = list.end(); it0 != end; ++it0, ++it1)
	{
		CopyValue(it1, *it0);
	}
}

template<class Type> 
inline Vector<Type>::Vector(const Vector<Type>& other) : size(other.size), capacity(other.size), array((Type*)malloc(capacity * sizeof(Type)))
{
	MoveValues(array, other.array, size);
}

template<class Type> 
inline Vector<Type>::Vector(Vector<Type>&& other) noexcept : size(other.size), capacity(other.capacity), array(other.array)
{
	other.size = 0;
	other.capacity = 0;
	other.array = nullptr;
}

template<class Type> 
inline Vector<Type>& Vector<Type>::operator=(const Vector<Type>& other)
{
	Destroy();

	size = other.size;
	capacity = size;
	array = (Type*)malloc(capacity * sizeof(Type));

	MoveValues(array, other.array, size);

	return *this;
}

template<class Type> 
inline Vector<Type>& Vector<Type>::operator=(Vector<Type>&& other) noexcept
{
	Destroy();

	size = other.size;
	capacity = other.capacity;
	array = other.array;

	other.size = 0;
	other.capacity = 0;
	other.array = nullptr;

	return *this;
}

template<class Type> 
inline Vector<Type>::~Vector() { Destroy(); }

template<class Type> 
inline void Vector<Type>::Destroy()
{
	if constexpr (std::is_destructible_v<Type>)
	{
		for (Type* it = array, *end = array + size; it != end; ++it) { it->~Type(); }
	}

	if(array) { free(array); array = nullptr; }

	size = 0;
	capacity = 0;
}

template<class Type> 
inline Type& Vector<Type>::Push(const Type& value)
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	return *CopyValue(array + size++, value);
}

template<class Type> inline Type& Vector<Type>::Push(Type&& value) noexcept
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	return *MoveValue(array + size++, std::move(value));
}

template<class Type> inline void Vector<Type>::Pop()
{
	if (size)
	{
		if constexpr (std::is_destructible_v<Type>) { (array + size)->~Type(); }

		--size;
	}
}

template<class Type> inline void Vector<Type>::Pop(Type& value)
{
	if (size)
	{
		MoveValue(&value, std::move(array[--size]));
	}
}

template<class Type>
inline Type& Vector<Type>::Insert(U64 index, const Type& value)
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	MoveValues(array + index + 1, array + index, (size - index));
	++size;
	return *CopyValue(array + index, value);
}

template<class Type>
inline Type& Vector<Type>::Insert(U64 index, Type&& value) noexcept
{
	if (size == capacity) { Reserve((capacity + 1) * 2); }

	MoveValues(array + index + 1, array + index, (size - index));
	++size;
	return *MoveValue(array + index, std::move(value));
}

template<class Type>
inline void Vector<Type>::Insert(U64 index, const Vector<Type>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	MoveValues(array + index + other.size, array + index, (size - index));
	CopyValues(array + index, other.array, other.size);

	size += other.size;
}

template<class Type>
inline void Vector<Type>::Insert(U64 index, Vector<Type>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	MoveValues(array + index + other.size, array + index, (size - index));
	MoveValues(array + index, other.array, other.size);

	size += other.size;
	other.Destroy();
}

template<class Type> inline void Vector<Type>::Remove(U64 index)
{
	if constexpr (std::is_destructible_v<Type>) { (array + index)->~Type(); }
	MoveValues(array + index, array + index + 1, (size - index));

	--size;
}

template<class Type> inline void Vector<Type>::Remove(U64 index, Type& value)
{
	MoveValue(&value, std::move(array[index]));
	MoveValues(array + index, array + index + 1, (size - index));

	--size;
}

template<class Type> inline void Vector<Type>::RemoveSwap(U64 index)
{
	if constexpr (std::is_destructible_v<Type>) { (array + index)->~Type(); }
	MoveValue(array + index, std::move(array[--size]));
}

template<class Type> inline void Vector<Type>::RemoveSwap(U64 index, Type& value)
{
	MoveValue(value, std::move(array[index]));
	MoveValue(array + index, std::move(array[--size]));
}

template<class Type> inline void Vector<Type>::Erase(U64 index0, U64 index1)
{
	if (index0 >= index1) { return; }

	if constexpr (std::is_destructible_v<Type>)
	{
		for (Type* it = array + index0, *end = array + index1; it != end; ++it) { it->~Type(); }
	}

	MoveValues(array + index0, array + index1, (size - index1));

	size -= index1 - index0;
}

template<class Type> inline void Vector<Type>::Steal(U64 index0, U64 index1, Vector<Type>& other)
{
	if (index0 >= index1) { return; }

	U64 stealSize = index1 - index0;
	other.Reserve(other.size + stealSize);

	MoveValues(other.array + other.size, array + index0, stealSize);
	MoveValues(array + index0, array + index1, (size - index1));

	other.size += stealSize;
	size -= stealSize;
}

template<class Type> inline void Vector<Type>::Split(U64 index, Vector<Type>& other)
{
	other.Destroy();
	other.Reserve(size - index);
	other.size = other.capacity;

	MoveValues(other.array, array + index, other.size);

	size -= index;
}

template<class Type> inline void Vector<Type>::Merge(const Vector<Type>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	CopyValues(array + size, other.array, other.size);
	size += other.size;
}

template<class Type> inline void Vector<Type>::Merge(Vector<Type>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	MoveValues(array + size, other.array, other.size);
	size += other.size;

	other.Destroy();
}

template<class Type> inline Vector<Type>& Vector<Type>::operator+=(const Vector<Type>& other)
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	CopyValues(array + size, other.array, other.size);
	size += other.size;

	return *this;
}

template<class Type> inline Vector<Type>& Vector<Type>::operator+=(Vector<Type>&& other) noexcept
{
	if (size + other.size > capacity) { Reserve(size + other.size); }

	MoveValues(array + size, other.array, other.size);
	size += other.size;

	other.Destroy();

	return *this;
}

template<class Type>
inline void Vector<Type>::SearchFor(Predicate predicate, Vector<Type>& other)
{
	other.Reserve(size);
	other.size = 0;

	for (Type* t = array, *end = array + size; t != end; ++t)
	{
		if (predicate(*t)) { other.Push(*t); }
	}
}

template<class Type>
inline void Vector<Type>::SearchForIndices(Predicate predicate, Vector<U64>& other)
{
	other.Reserve(size);
	other.size = 0;

	U64 i = 0;
	for (Type* t = array, *end = array + size; t != end; ++t, ++i)
	{
		if (predicate(*t)) { other.Push(i); }
	}
}

template<class Type>
inline U64 Vector<Type>::SearchCount(Predicate predicate)
{
	U64 i = 0;
	for (Type* t = array, *end = array + size; t != end; ++t)
	{
		if (predicate(*t)) { ++i; }
	}

	return i;
}

template<class Type>
inline U64 Vector<Type>::RemoveAll(Predicate predicate)
{
	U64 i = 0;
	U64 count = 0;
	for (Type* t = array, *end = array + size; t != end;)
	{
		if (predicate(*t)) { Remove(i); ++count; }
		else { ++t; ++i; }
	}

	return count;
}

template<class Type>
inline void Vector<Type>::RemoveAll(Predicate predicate, Vector<Type>& other)
{
	other.Reserve(size);
	other.size = 0;

	U64 i = 0;
	U64 count = 0;
	for (Type* t = array, *end = array + size; t != end;)
	{
		if (predicate(*t))
		{
			Type value;
			Remove(i, value);
			other.Push(std::move(value));
			++count;
		}
		else { ++t; ++i; }
	}
}

template<class Type>
inline U64 Vector<Type>::RemoveSwapAll(Predicate predicate)
{
	U64 i = 0;
	U64 count = 0;
	for (Type* t = array, *end = array + size; t != end;)
	{
		if (predicate(*t)) { RemoveSwap(i); ++count; }
		else { ++t; ++i; }
	}

	return i;
}

template<class Type>
inline void Vector<Type>::RemoveSwapAll(Predicate predicate, Vector<Type>& other)
{
	other.Reserve(size);
	other.size = 0;

	U64 i = 0;
	U64 count = 0;
	for (Type* t = array, *end = array + size; t != end;)
	{
		if (predicate(*t))
		{
			Type value;
			RemoveSwap(i, value);
			other.Push(std::move(value));
			++count;
		}
		else { ++t; ++i; }
	}
}

template<class Type>
inline Type* Vector<Type>::Find(Predicate predicate)
{
	for (Type* t = array, *end = array + size; t != end; ++t)
	{
		if (predicate(t)) { return t; }
	}

	return nullptr;
}

template<class Type>
U64 Vector<Type>::SortedInsert(Compare predicate, const Type& value)
{
	U64 i = 0;
	for (Type* t = array, *end = array + size; t != end; ++t, ++i)
	{
		if (predicate(value, *t))
		{
			Insert(i, value);

			return i;
		}
	}

	U64 index = size;

	Push(value);

	return index;
}

template<class Type>
U64 Vector<Type>::SortedInsert(Compare predicate, Type&& value) noexcept
{
	U64 i = 0;
	for (Type* t = array, *end = array + size; t != end; ++t, ++i)
	{
		if (predicate(value, *t))
		{
			Insert(i, std::move(value));

			return i;
		}
	}

	U64 index = size;

	Push(std::move(value));

	return index;
}

template<class Type>
inline void Vector<Type>::Reserve(U64 capacity)
{
	if constexpr (std::is_destructible_v<Type>)
	{
		Type* temp = (Type*)malloc(capacity * sizeof(Type));
		MoveValues(temp, array, size);
		free(array);
		array = temp;
	}
	else
	{
		realloc(array, capacity);
	}

	this->capacity = capacity;
}

template<class Type>
inline void Vector<Type>::Resize(U64 size)
{
	if (size > capacity) { Reserve(size); }

	this->size = size;
}

template<class Type>
inline void Vector<Type>::Resize(U64 size, const Type& value)
{
	if (size > capacity) { Reserve(size); }
	this->size = size;

	for (U64 i = 0; i < size; ++i) { CopyValue(array + i, value); }
}

template<class Type>
inline bool Vector<Type>::Contains(const Type& value) const
{
	for (Type* t = array, *end = array + size; t != end; ++t)
	{
		if (*t == value) { return true; }
	}

	return false;
}

template<class Type>
inline U64 Vector<Type>::Count(const Type& value) const
{
	U64 count = 0;
	for (Type* t = array, *end = array + size; t != end; ++t)
	{
		if (*t == value) { ++count; }
	}

	return count;
}

template<class Type>
inline U64 Vector<Type>::Find(const Type& value) const
{
	U64 index = 0;
	for (Type* t = array; index < size; ++index, ++t)
	{
		if (*t == value) { return index; }
	}

	return -1;
}

template<class Type>
inline bool Vector<Type>::operator==(const Vector& other) const
{
	if (size != other.size) { return false; }

	for (Type* it0 = array, *it1 = other.array, *end = array + size; it0 != end; ++it0, ++it1)
	{
		if (*it0 != *it0) { return false; }
	}

	return true;
}

template<class Type>
inline bool Vector<Type>::operator!=(const Vector& other) const
{
	if (size != other.size) { return true; }

	for (Type* it0 = array, *it1 = other.array, *end = array + size; it0 != end; ++it0, ++it1)
	{
		if (*it0 != *it0) { return true; }
	}

	return false;
}
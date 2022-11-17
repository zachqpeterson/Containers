#pragma once

#include "Defines.h"
#include "Initiazer.h"

template<typename T>
struct List
{
	struct Node
	{
		T value;
		T* next;
	};

	struct Iterator
	{
	public:
		Iterator(Node* ptr) : ptr{ ptr } {}

	private:
		Node* ptr;
	};

public:
	List();
	List(const List& other);
	List(List&& other);
	List& operator=(const List& other);
	List& operator=(List&& other);
	template<T... Args> constexpr List(const Initializer<T, Args...>& init);
	template<T... Args> constexpr List& operator=(const Initializer<T, Args...>& init);

	void PushBack(const T& value);
	void PushBack(T&& value);
	void PopBack();
	void PushFront(const T& value);
	void PushFront(T&& value);
	void PopFront();
	void Insert(U64 index, const T& value);
	void Insert(U64 index, T&& value);
	void Insert(Iterator& position, const T& value);
	void Insert(Iterator& position, T&& value);
	void Insert(U64 index, const List& other);
	void Insert(Iterator& position, const List& other);
	void Remove(U64 index);
	void Remove(Iterator& position);
	void Remove(Iterator& start, Iterator& end);

	template<T... Args> PushBack(const Args&... args);
	template<T... Args> PushFront(const Args&... args);
	template<T... Args> Insert(U64 index, const Args&... args);
	template<T... Args> Insert(Iterator& position, const Args&... args);

	void Reverse();
	void Splice(U64 index);
	void Splice(Iterator& position);

private:
	U64 size;
	T* head;
	T* tail;
};
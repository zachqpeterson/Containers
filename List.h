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

		T& operator* () const { return ptr->value; }
		T* operator-> () { return &ptr->value; }

		Iterator& operator++ () { ptr = ptr->next; return *this; }
		Iterator& operator-- () { ptr = ptr->prev; return *this; }

		Iterator operator++ (int)
		{
			Iterator temp = *this;
			ptr = ptr->next;
			return temp;
		}

		Iterator operator-- (int)
		{
			Iterator temp = *this;
			ptr = ptr->prev;
			return temp;
		}

		Iterator& operator+= (int i)
		{
			for (int j = 0; j < i; ++j) { ptr = ptr->next; }
			return *this;
		}

		Iterator& operator-= (int i)
		{
			for (int j = 0; j < i; ++j) { ptr = ptr->prev; }
			return *this;
		}

		Iterator operator+ (int i)
		{
			Node* p = ptr;

			for (int j = 0; j < i; ++j) { p = p->next; }
			return p;
		}

		Iterator operator- (int i)
		{
			Node* p = ptr;
			for (int j = 0; j < i; ++j) { p = p->prev; }
			return p;
		}

		friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
		friend bool operator< (const Iterator& a, const Iterator& b) { return a.ptr < b.ptr; }
		friend bool operator> (const Iterator& a, const Iterator& b) { return a.ptr > b.ptr; }
		friend bool operator<= (const Iterator& a, const Iterator& b) { return a.ptr <= b.ptr; }
		friend bool operator>= (const Iterator& a, const Iterator& b) { return a.ptr >= b.ptr; }

		operator bool() { return ptr; }

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

	~List();
	void Destroy();
	void* operator new(U64 size);
	void operator delete(void* ptr);

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
	void RemoveAll(const T& value);

	template<T... Args> PushBack(const Args&... args);
	template<T... Args> PushFront(const Args&... args);
	template<T... Args> Insert(U64 index, const Args&... args);
	template<T... Args> Insert(Iterator& position, const Args&... args);
	template<typename Predicate> RemoveIf(Predicate predicate);

	void Reverse();
	void Splice(U64 index, List& other);
	void Splice(Iterator& position, List& other);
	void Merge(List& other);

private:
	U64 size;
	T* head;
	T* tail;
};

template<typename T> inline List<T>::List() : size{ 0 }, head{ nullptr }, tail{ nullptr } {}

template<typename T> inline List<T>::List(const List<T>& other) : size{ other.size }, head{ nullptr }, tail{ nullptr }
{
	Node* node = other.head;
	Node* newNode = head;
	while (node)
	{
		newNode = (Node*)malloc(sizeof(Node));
		newNode->value = node->value;
		node = node->next;
		newNode = newNode->next;
	}
}

template<typename T> inline List<T>::List(List<T>&& other) : size{ other.size }, head{ other.head }, tail{ other.tail }
{
	other.head = nullptr;
	other.tail = nullptr;
	other.size = 0;
}

template<typename T> inline List<T>& List<T>::operator=(const List<T>& other)
{
	size = other.size;

	Node* node = other.head;
	Node* newNode = head;
	while (node)
	{
		newNode = (Node*)malloc(sizeof(Node));
		newNode->value = node->value;
		node = node->next;
		newNode = newNode->next;
	}
}

template<typename T> inline List<T>& List<T>::operator=(List<T>&& other)
{
	head = other.head;
	tail = other.tail;
	size = other.size;

	other.head = nullptr;
	other.tail = nullptr;
	other.size = 0;
}

template<typename T> template<T... Args> inline constexpr List<T>::List(const Initializer<T, Args...>& init)
{

}

template<typename T> template<T... Args> inline constexpr List<T>& List<T>::operator=(const Initializer<T, Args...>& init)
{

}

template<typename T> inline List<T>::~List()
{

}

template<typename T> inline void List<T>::Destroy()
{

}

template<typename T> inline void* List<T>::operator new(U64 size)
{

}

template<typename T> inline void List<T>::operator delete(void* ptr)
{

}

template<typename T> inline void List<T>::PushBack(const T& value)
{

}

template<typename T> inline void List<T>::PushBack(T&& value)
{

}

template<typename T> inline void List<T>::PopBack()
{

}

template<typename T> inline void List<T>::PushFront(const T& value)
{

}

template<typename T> inline void List<T>::PushFront(T&& value)
{

}

template<typename T> inline void List<T>::PopFront()
{

}

template<typename T> inline void List<T>::Insert(U64 index, const T& value)
{

}

template<typename T> inline void List<T>::Insert(U64 index, T&& value)
{

}

template<typename T> inline void List<T>::Insert(Iterator& position, const T& value)
{

}

template<typename T> inline void List<T>::Insert(Iterator& position, T&& value)
{

}

template<typename T> inline void List<T>::Insert(U64 index, const List<T>& other)
{

}

template<typename T> inline void List<T>::Insert(Iterator& position, const List<T>& other)
{

}

template<typename T> inline void List<T>::Remove(U64 index)
{

}

template<typename T> inline void List<T>::Remove(Iterator& position)
{

}

template<typename T> inline void List<T>::Remove(Iterator& start, Iterator& end)
{

}

template<typename T> inline void List<T>::RemoveAll(const T& value)
{

}

template<typename T> template<T... Args> inline void List<T>::PushBack(const Args&... args)
{

}

template<typename T> template<T... Args> inline void List<T>::PushFront(const Args&... args)
{

}

template<typename T> template<T... Args> inline void List<T>::Insert(U64 index, const Args&... args)
{

}

template<typename T> template<T... Args> inline void List<T>::Insert(Iterator& position, const Args&... args)
{

}

template<typename T> template<typename Predicate> inline void List<T>::RemoveIf(Predicate predicate)
{

}

template<typename T> inline void List<T>::Reverse()
{

}

template<typename T> inline void List<T>::Splice(U64 index, List<T>& other)
{

}

template<typename T> inline void List<T>::Splice(Iterator& position, List<T>& other)
{

}

template<typename T> inline void List<T>::Merge(List<T>& other)
{

}

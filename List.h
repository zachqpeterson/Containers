#pragma once

#include "Defines.hpp"
#include "Initiazer.h"

template<typename T>
struct List
{
	struct Node
	{
		Node(const T& value) : value{ value }, next{ nullptr }, prev{ nullptr } {}

		T value;
		Node* next;
		Node* prev;
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
	void InsertBack(Iterator& position, const T& value);
	void InsertFront(Iterator& position, const T& value);
	void InsertBack(Iterator& position, T&& value);
	void InsertFront(Iterator& position, T&& value);
	void Insert(U64 index, const List& other);
	void InsertBack(Iterator& position, const List& other);
	void InsertBack(Iterator& position, List&& other);
	void InsertFront(Iterator& position, const List& other);
	void InsertFront(Iterator& position, List&& other);
	void Remove(U64 index);
	void Remove(Iterator& position);
	void Remove(Iterator& start, Iterator& end);
	void RemoveAll(const T& value);

	/*template<T... Args> void PushBack(const Args&... args);
	template<T... Args> void PushFront(const Args&... args);
	template<T... Args> void Insert(U64 index, const Args&... args);
	template<T... Args> void Insert(Iterator& position, const Args&... args);*/
	template<typename Predicate> U64 RemoveIf(Predicate);

	void Reverse();
	void Splice(U64 index, List& other);
	void Splice(Iterator& position, List& other);
	void Merge(List& other);

	Iterator begin() { return { head }; }
	Iterator end() { if (tail) { return { tail->next }; } return { tail }; }
	Iterator begin() const { return { head }; }
	Iterator end() const { if (tail) { return { tail->next }; } return { tail }; }

	U64 Size() const { return size; }
	T& Front() { return head->value; }
	const T& Front() const { return head->value; }
	T& Back() { return tail->value; }
	const T& Back() const { return tail->value; }

private:
	U64 size;
	Node* head;
	Node* tail;

	inline void EraseNode(Node* node);
};

template<typename T> inline List<T>::List() : size{ 0 }, head{ nullptr }, tail{ nullptr } {}

template<typename T> inline List<T>::List(const List<T>& other) : size{ other.size }, head{ nullptr }, tail{ nullptr }
{
	Node* node = other.head;
	Node* newNode = head;
	while (node)
	{
		PushBack(node->value);
		node->next;
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
	while (node)
	{
		PushBack(node->value);
		node->next;
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
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = value;
	newNode->next = nullptr;
	++size;

	if (head)
	{
		tail->next = newNode;
		newNode->prev = tail;
	}
	else
	{
		head = newNode;
		head->prev = nullptr;
	}

	tail = newNode;
}

template<typename T> inline void List<T>::PushBack(T&& value)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = Move(value);
	newNode->next = nullptr;
	++size;

	if (head)
	{
		tail->next = newNode;
		newNode->prev = tail;
	}
	else 
	{ 
		head = newNode;
		head->prev = nullptr;
	}

	tail = newNode;
}

template<typename T> inline void List<T>::PopBack()
{
	if (size)
	{
		Node* node = tail;
		tail = tail->prev;
		EraseNode(node);
	}
}

template<typename T> inline void List<T>::PushFront(const T& value)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = value;
	newNode->prev = nullptr;
	++size;

	if (head)
	{
		head->prev = newNode;
		newNode->next = head;
	}
	else 
	{ 
		tail = newNode; 
		tail->next = nullptr;
	}

	head = newNode;
}

template<typename T> inline void List<T>::PushFront(T&& value)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = Move(value);
	newNode->prev = nullptr;
	++size;

	if (head)
	{
		head->prev = newNode;
		newNode->next = head;
	}
	else
	{
		tail = newNode;
		tail->next = nullptr;
	}

	head = newNode;
}

template<typename T> inline void List<T>::PopFront()
{
	if (size)
	{
		Node* node = head;
		head = head->next;
		EraseNode(node);
	}
}

template<typename T> inline void List<T>::Insert(U64 index, const T& value)
{
	if (index == 0) { PushFront(value); }
	else if (index == size) { PushBack(value); }
	else
	{
		Node* node = head;
		for (U64 i = 1; i < index; ++i) { node = node->next; }

		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = value;

		node->next->prev = newNode;
		newNode->next = node->next;
		node->next = newNode;
		newNode->prev = node;

		++size;
	}
}

template<typename T> inline void List<T>::Insert(U64 index, T&& value)
{
	if (index == 0) { PushFront(value); }
	else if (index == size) { PushBack(value); }
	else
	{
		Node* node = head;
		for (U64 i = 1; i < index; ++i) { node = node->next; }

		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = Move(value);

		node->next->prev = newNode;
		newNode->next = node->next;
		node->next = newNode;
		newNode->prev = node;

		++size;
	}
}

template<typename T> inline void List<T>::InsertBack(Iterator& position, const T& value)
{
	if (position == end() || position == end() - 1) { PushBack(); }
	else
	{
		Node* node = position->ptr;
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = value;

		node->next->prev = newNode;
		newNode->next = node->next;
		node->next = newNode;
		newNode->prev = node;

		++size;
	}
}

template<typename T> inline void List<T>::InsertFront(Iterator& position, const T& value)
{
	if (position == begin()) { PushFront(); }
	else if (position == end()) { PushBack(); }
	else
	{
		Node* node = position->ptr;
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = value;

		node->prev->next = newNode;
		newNode->prev = node->prev;
		node->prev = newNode;
		newNode->next = node;

		++size;
	}
}

template<typename T> inline void List<T>::InsertBack(Iterator& position, T&& value)
{
	if (position == end() || position == end() - 1) { PushBack(); }
	else
	{
		Node* node = position->ptr;
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = Move(value);

		node->next->prev = newNode;
		newNode->next = node->next;
		node->next = newNode;
		newNode->prev = node;

		++size;
	}
}

template<typename T> inline void List<T>::InsertFront(Iterator& position, T&& value)
{
	if (position == begin()) { PushFront(); }
	else
	{
		Node* node = position->ptr;
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->value = Move(value);

		node->prev->next = newNode;
		newNode->prev = node->prev;
		node->prev = newNode;
		newNode->next = node;

		++size;
	}
}

template<typename T> inline void List<T>::Insert(U64 index, const List<T>& other)
{

}

template<typename T> inline void List<T>::InsertBack(Iterator& position, const List<T>& other)
{

}

template<typename T> inline void List<T>::InsertBack(Iterator& position, List<T>&& other)
{

}

template<typename T> inline void List<T>::InsertFront(Iterator& position, const List<T>& other)
{

}

template<typename T> inline void List<T>::InsertFront(Iterator& position, List<T>&& other)
{

}

template<typename T> inline void List<T>::Remove(U64 index)
{
	Node* node = head;
	for (U64 i = 0; i < index; ++i) { node = node->next; }

	EraseNode(node);
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

//template<typename T> template<T... Args> inline void List<T>::PushBack(const Args&... args)
//{
//
//}
//
//template<typename T> template<T... Args> inline void List<T>::PushFront(const Args&... args)
//{
//
//}
//
//template<typename T> template<T... Args> inline void List<T>::Insert(U64 index, const Args&... args)
//{
//
//}
//
//template<typename T> template<T... Args> inline void List<T>::Insert(Iterator& position, const Args&... args)
//{
//
//}

template<typename T> template<typename Predicate> inline U64 List<T>::RemoveIf(Predicate predicate)
{
	U64 removed = 0;

	for (Iterator it = begin(), last = end(); it != last;)
	{
		T& value = *it;

		if (predicate(value))
		{
			++removed;
			++it;
			EraseNode(it.ptr);
		}
		else { ++it; }
	}

	return removed;
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


template<typename T> inline void List<T>::EraseNode(Node* node)
{
	if (node->next) { node->next->prev = node->prev; }
	else { tail = node->prev; }

	if (node->prev) { node->prev->next = node->next; }
	else { head = node->next; }

	free(node);
	--size;
}
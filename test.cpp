#include "Vector.hpp"
#include "List.h"
#include "String.hpp"
#include "Initiazer.h"
#include "Defines.hpp"

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <list>

#define BEGIN_TEST				\
bool passed = false;			\
LARGE_INTEGER start;			\
QueryPerformanceCounter(&start);

template<U64 Count> constexpr U64 Length(const char(&)[Count]) { return Count; }

#define END_TEST																		\
LARGE_INTEGER end;																		\
QueryPerformanceCounter(&end);															\
F64 time = (F64)(end.QuadPart - start.QuadPart) / (F64)freq.QuadPart;					\
constexpr U64 len = Length(__FUNCTION__);												\
char name[len + 2] = __FUNCTION__;														\
name[len] = ':';																		\
name[len + 1] = '\0';																	\
printf("%-35s %s  |  Time taken: %fs\n", name, passed ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m", time);

template<typename T>
bool Compare(const T* a, const T* b, U64 length)
{
	while (length--) { if (*a++ != *b++) { return false; } }

	return true;
}

LARGE_INTEGER freq;

bool Pred0(U64 i) { return i == 2; }

bool Pred1(U64 i) { return i < 5; }

#pragma region Vector Tests
void VectorInit_Blank()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0;
	Vector<I32*> v1;

	passed = v0.Size() == 0 && v0.Capacity() == 0 && v0.Data() == nullptr &&
		v1.Size() == 0 && v1.Capacity() == 0 && v1.Data() == nullptr;

	/*** END TEST ***/
	END_TEST
}

void VectorInit_Capacity()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	Vector<I32*> v1(10);

	passed = v0.Size() == 0 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 0 && v1.Capacity() == 10 && v1.Data();

	for (I32 i : v0)
	{
		passed = false;
	}

	for (I32* i : v1)
	{
		passed = false;
	}

	/*** END TEST ***/

	END_TEST
}

void VectorInit_Size()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32*> v1(10, new I32(1));

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data();

	for (I32 i : v0)
	{
		if (i != 1) { passed = false; }
	}

	for (I32* i : v1)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorInit_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(v0);
	Vector<I32*> v2(10, new I32(1));
	Vector<I32*> v3(v2);

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data() &&
		v2.Size() == 10 && v2.Capacity() == 10 && v2.Data() &&
		v3.Size() == 10 && v3.Capacity() == 10 && v3.Data();

	for (I32 i : v0)
	{
		if (i != 1) { passed = false; }
	}

	for (I32 i : v1)
	{
		if (i != 1) { passed = false; }
	}

	for (I32* i : v2)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	for (I32* i : v3)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorInit_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(Move(v0));
	Vector<I32*> v2(10, new I32(1));
	Vector<I32*> v3(Move(v2));

	passed = v0.Size() == 0 && v0.Capacity() == 0 && v0.Data() == nullptr &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data() &&
		v2.Size() == 0 && v2.Capacity() == 0 && v2.Data() == nullptr &&
		v3.Size() == 10 && v3.Capacity() == 10 && v3.Data();

	for (I32 i : v0)
	{
		passed = false;
	}

	for (I32 i : v1)
	{
		if (i != 1) { passed = false; }
	}

	for (I32* i : v2)
	{
		passed = false;
	}

	for (I32* i : v3)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorInit_Initializer()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Initializer<I32, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1> init;

	Vector<I32> v0(init);

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data();

	for (I32 i : v0)
	{
		if (i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorAssign_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1 = v0;
	Vector<I32*> v2(10, new I32(1));
	Vector<I32*> v3 = v2;

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data() &&
		v2.Size() == 10 && v2.Capacity() == 10 && v2.Data() &&
		v3.Size() == 10 && v3.Capacity() == 10 && v3.Data();

	for (I32 i : v0)
	{
		if (i != 1) { passed = false; }
	}

	for (I32 i : v1)
	{
		if (i != 1) { passed = false; }
	}

	for (I32* i : v2)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	for (I32* i : v3)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorAssign_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1 = Move(v0);
	Vector<I32*> v2(10, new I32(1));
	Vector<I32*> v3 = Move(v2);

	passed = v0.Size() == 0 && v0.Capacity() == 0 && v0.Data() == nullptr &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data() &&
		v2.Size() == 0 && v2.Capacity() == 0 && v2.Data() == nullptr &&
		v3.Size() == 10 && v3.Capacity() == 10 && v3.Data();

	for (I32 i : v0)
	{
		passed = false;
	}

	for (I32 i : v1)
	{
		if (i != 1) { passed = false; }
	}

	for (I32* i : v2)
	{
		passed = false;
	}

	for (I32* i : v3)
	{
		if (i == nullptr || *i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorAssign_Initializer()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Initializer<I32, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1> init;

	Vector<I32> v0 = init;

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data();

	for (I32 i : v0)
	{
		if (i != 1) { passed = false; }
	}

	/*** END TEST ***/

	END_TEST
}

void VectorDestroy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(1, 10);

	v0.Destroy();

	passed = v0.Size() == 0 && v0.Capacity() == 0 && v0.Data() == nullptr;

	for (I32 i : v0)
	{
		passed = false;
	}

	/*** END TEST ***/

	END_TEST
}

void VectorNewDelete()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32>* v0 = new Vector<I32>(10, 1);

	passed = v0 && v0->Size() == 10 && v0->Capacity() == 10 && v0->Data();

	for (I32 i : *v0)
	{
		if (i != 1) { passed = false; }
	}

	delete v0;

	/*** END TEST ***/

	END_TEST
}

void VectorPush_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0;

	for (I32 i = 0; i < 10; ++i)
	{
		v0.Push(i);
		passed &= v0.Size() == i + 1 && v0.Capacity() >= v0.Size() && v0.Data() && v0[i] == i;
	}

	/*** END TEST ***/

	END_TEST
}

void VectorPush_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0;

	for (I32 i = 0; i < 10; ++i)
	{
		v0.Push(Move(i));
		passed &= v0.Size() == i + 1 && v0.Capacity() >= v0.Size() && v0.Data() && v0[i] == i;
	}

	/*** END TEST ***/

	END_TEST
}

void VectorPop()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	U64 cap = v0.Capacity();

	for (U64 i = 10; i > 0; --i)
	{
		v0.Pop();
		passed &= v0.Size() == i - 1 && v0.Capacity() == cap && v0.Data();
	}

	/*** END TEST ***/

	END_TEST
}

void VectorPop_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	U64 cap = v0.Capacity();

	for (U64 i = 10; i > 0; --i)
	{
		I32 j;
		v0.Pop(j);
		passed &= j == 1 && v0.Size() == i - 1 && v0.Capacity() == cap && v0.Data();
	}

	/*** END TEST ***/

	END_TEST
}

void VectorPop_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	U64 cap = v0.Capacity();

	for (U64 i = 10; i > 0; --i)
	{
		I32 j;
		v0.Pop(Move(j));
		passed &= j == 1 && v0.Size() == i - 1 && v0.Capacity() == cap && v0.Data();
	}

	/*** END TEST ***/

	END_TEST
}

void VectorInsert_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	I32 i = 2;
	v0.Insert(5, i);

	passed = v0.Size() == 11 && v0.Capacity() >= v0.Size() && v0.Data() && v0[5] == 2;

	v0.Insert(0, i);

	passed &= v0.Size() == 12 && v0.Capacity() >= v0.Size() && v0.Data() && v0[0] == 2;

	v0.Insert(v0.Size(), i);

	passed &= v0.Size() == 13 && v0.Capacity() >= v0.Size() && v0.Data() && v0[12] == 2;

	/*** END TEST ***/

	END_TEST
}

void VectorInsert_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	v0.Insert(5, 2);

	passed = v0.Size() == 11 && v0.Capacity() >= v0.Size() && v0.Data() && v0[5] == 2;

	v0.Insert(0, 2);

	passed &= v0.Size() == 12 && v0.Capacity() >= v0.Size() && v0.Data() && v0[0] == 2;

	v0.Insert(v0.Size(), 2);

	passed &= v0.Size() == 13 && v0.Capacity() >= v0.Size() && v0.Data() && v0[12] == 2;

	/*** END TEST ***/

	END_TEST
}

void VectorInsert_CopyVector()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);
	Vector<I32> v2(10, 3);
	Vector<I32> v3(10, 4);

	v0.Insert(5, v1);

	passed = v0.Size() == 20 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 5; i < 15; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 15; i < 20; ++i) { passed &= v0[i] == 1; }

	v0.Insert(0, v2);

	passed &= v0.Size() == 30 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v2.Size() == 10 && v2.Capacity() == 10 && v2.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 3; }
	for (I32 i = 10; i < 15; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 15; i < 25; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 25; i < 30; ++i) { passed &= v0[i] == 1; }

	v0.Insert(v0.Size(), v3);

	passed &= v0.Size() == 40 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v3.Size() == 10 && v3.Capacity() == 10 && v3.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 3; }
	for (I32 i = 10; i < 15; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 15; i < 25; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 25; i < 30; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 30; i < 40; ++i) { passed &= v0[i] == 4; }

	/*** END TEST ***/

	END_TEST
}

void VectorInsert_MoveVector()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);
	Vector<I32> v2(10, 3);
	Vector<I32> v3(10, 4);

	v0.Insert(5, Move(v1));

	passed = v0.Size() == 20 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v1.Size() == 0 && v1.Capacity() == 0 && !v1.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 5; i < 15; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 15; i < 20; ++i) { passed &= v0[i] == 1; }

	v0.Insert(0, Move(v2));

	passed &= v0.Size() == 30 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v2.Size() == 0 && v2.Capacity() == 0 && !v2.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 3; }
	for (I32 i = 10; i < 15; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 15; i < 25; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 25; i < 30; ++i) { passed &= v0[i] == 1; }

	v0.Insert(v0.Size(), Move(v3));

	passed &= v0.Size() == 40 && v0.Capacity() >= v0.Size() && v0.Data() &&
		v3.Size() == 0 && v3.Capacity() == 0 && !v3.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 3; }
	for (I32 i = 10; i < 15; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 15; i < 25; ++i) { passed &= v0[i] == 2; }
	for (I32 i = 25; i < 30; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 30; i < 40; ++i) { passed &= v0[i] == 4; }

	/*** END TEST ***/

	END_TEST
}

void VectorRemove()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	v0.Remove(5);

	passed = v0.Size() == 9 && v0.Capacity() == 10 && v0.Data() && v0[5] == 7;

	v0.Remove(0);

	passed &= v0.Size() == 8 && v0.Capacity() == 10 && v0.Data() && v0[0] == 2;

	v0.Remove(v0.Size());

	passed &= v0.Size() == 7 && v0.Capacity() == 10 && v0.Data() && v0[6] == 9;

	/*** END TEST ***/

	END_TEST
}

void VectorRemove_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	I32 i;
	v0.Remove(5, i);

	passed = v0.Size() == 9 && v0.Capacity() == 10 && v0.Data() && v0[5] == 7 && i == 6;

	v0.Remove(0, i);

	passed &= v0.Size() == 8 && v0.Capacity() == 10 && v0.Data() && v0[0] == 2 && i == 1;

	v0.Remove(v0.Size() - 1, i);

	passed &= v0.Size() == 7 && v0.Capacity() == 10 && v0.Data() && v0[6] == 9 && i == 10;

	/*** END TEST ***/

	END_TEST
}

void VectorRemove_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	I32 i;
	v0.Remove(5, Move(i));

	passed = v0.Size() == 9 && v0.Capacity() == 10 && v0.Data() && v0[5] == 7 && i == 6;

	v0.Remove(0, Move(i));

	passed &= v0.Size() == 8 && v0.Capacity() == 10 && v0.Data() && v0[0] == 2 && i == 1;

	v0.Remove(v0.Size() - 1, Move(i));

	passed &= v0.Size() == 7 && v0.Capacity() == 10 && v0.Data() && v0[6] == 9 && i == 10;

	/*** END TEST ***/

	END_TEST
}

void VectorErase()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	v0.Erase(5, 7);

	passed = v0.Size() == 8 && v0.Capacity() == 10 && v0.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == i + 1; }
	for (I32 i = 5; i < 8; ++i) { passed &= v0[i] == i + 3; }

	/*** END TEST ***/

	END_TEST
}

void VectorErase_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }
	Vector<I32> v1;

	v0.Erase(5, 7, v1);

	passed = v0.Size() == 8 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 2 && v1.Capacity() == 2 && v1.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == i + 1; }
	for (I32 i = 5; i < 8; ++i) { passed &= v0[i] == i + 3; }
	for (I32 i = 0; i < 2; ++i) { passed &= v1[i] == i + 6; }

	/*** END TEST ***/

	END_TEST
}

void VectorSplit()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1;

	v0.Split(5, v1);

	passed = v0.Size() == 5 && v0.Capacity() == 10 && v0.Data() &&
		v1.Size() == 5 && v1.Capacity() == 5 && v1.Data();

	/*** END TEST ***/

	END_TEST
}

void VectorMerge_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);

	v0.Merge(v1);

	passed = v0.Size() == 20 && v0.Capacity() == 20 && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 10; i < 20; ++i) { passed &= v0[i] == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorMerge_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);

	v0.Merge(Move(v1));

	passed = v0.Size() == 20 && v0.Capacity() == 20 && v0.Data() &&
		v1.Size() == 0 && v1.Capacity() == 0 && !v1.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 10; i < 20; ++i) { passed &= v0[i] == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorAdd_Copy()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);

	v0 += v1;

	passed = v0.Size() == 20 && v0.Capacity() == 20 && v0.Data() &&
		v1.Size() == 10 && v1.Capacity() == 10 && v1.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 10; i < 20; ++i) { passed &= v0[i] == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorAdd_Move()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);
	Vector<I32> v1(10, 2);

	v0 += Move(v1);

	passed = v0.Size() == 20 && v0.Capacity() == 20 && v0.Data() &&
		v1.Size() == 0 && v1.Capacity() == 0 && !v1.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 1; }
	for (I32 i = 10; i < 20; ++i) { passed &= v0[i] == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorPredicate_SearchFor()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<U64> v0(5, 1);
	v0.Push(2);
	v0.Push(1);
	v0.Push(2);
	v0.Push(2);
	v0.Push(1);
	Vector<U64> v1;

	v0.SearchFor(Pred0, v1);

	passed = v0.Size() == 10, v0.Capacity() >= v0.Size() && v1.Size() == 3 && v1.Capacity() == v0.Size() && v0.Data() && v1.Data();

	for (U64 i : v1) { passed &= i == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorPredicate_SearchForIndices()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<U64> v0(5, 1);
	v0.Push(2);
	v0.Push(1);
	v0.Push(2);
	v0.Push(2);
	v0.Push(1);
	Vector<U64> v1;

	v0.SearchForIndices(Pred0, v1);

	passed = v0.Size() == 10, v0.Capacity() >= v0.Size() && v1.Size() == 3 && v1.Capacity() == v0.Size() && v0.Data() && v1.Data() &&
		v1[0] == 5 && v1[1] == 7 && v1[2] == 8;

	/*** END TEST ***/

	END_TEST
}

void VectorPredicate_SearchCount()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<U64> v0(5, 1);
	v0.Push(2);
	v0.Push(1);
	v0.Push(2);
	v0.Push(2);
	v0.Push(1);

	U64 i = v0.SearchCount(Pred0);

	passed = v0.Size() == 10 && v0.Capacity() >= v0.Size() && v0.Data() && i == 3;

	/*** END TEST ***/

	END_TEST
}

void VectorPredicate_RemoveAll()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<U64> v0(5, 1);
	v0.Push(2);
	v0.Push(1);
	v0.Push(2);
	v0.Push(2);
	v0.Push(1);

	U64 i = v0.RemoveAll(Pred0);

	passed = v0.Size() == 7 && v0.Capacity() >= v0.Size() && v0.Data() && i == 3;

	for (U64 i : v0) { passed &= i == 1; }

	/*** END TEST ***/

	END_TEST
}

void VectorPredicate_RemoveAll_Other()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<U64> v0(5, 1);
	v0.Push(2);
	v0.Push(1);
	v0.Push(2);
	v0.Push(2);
	v0.Push(1);
	Vector<U64> v1;

	v0.RemoveAll(Pred0, v1);

	passed = v0.Size() == 7 && v0.Capacity() >= v0.Size() && v0.Data() && v1.Size() == 3 && v1.Capacity() == 10 && v1.Data();

	for (U64 i : v0) { passed &= i == 1; }
	for (U64 i : v1) { passed &= i == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorReserve()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(5, 1);

	v0.Reserve(10);

	passed = v0.Size() == 5 && v0.Capacity() == 10 && v0.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == 1; }

	/*** END TEST ***/

	END_TEST
}

void VectorResize()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(5, 1);

	v0.Resize(10);

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data();

	for (I32 i = 0; i < 5; ++i) { passed &= v0[i] == 1; }

	/*** END TEST ***/

	END_TEST
}

void VectorResize_Value()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(5, 1);

	v0.Resize(10, 2);

	passed = v0.Size() == 10 && v0.Capacity() == 10 && v0.Data();

	for (I32 i = 0; i < 10; ++i) { passed &= v0[i] == 2; }

	/*** END TEST ***/

	END_TEST
}

void VectorShrink()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(5, 1);

	v0.Push(1);
	v0.Shrink();

	passed = v0.Size() == 6 && v0.Capacity() == 6 && v0.Data();

	for (I32 i = 0; i < 6; ++i) { passed &= v0[i] == 1; }

	/*** END TEST ***/

	END_TEST
}

void VectorClear()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10, 1);

	v0.Clear();

	passed = v0.Size() == 0 && v0.Capacity() == 10 && v0.Data();

	/*** END TEST ***/

	END_TEST
}

void VectorContains()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	passed = v0.Contains(5) && !v0.Contains(11);

	/*** END TEST ***/

	END_TEST
}

void VectorCount()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i / 5); }

	passed = v0.Count(0) == 4 && v0.Count(1) == 5 && v0.Count(2) == 1;

	/*** END TEST ***/

	END_TEST
}

void VectorFind()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	passed = v0.Find(5) == 4 && v0.Find(11) == (U64)-1;

	/*** END TEST ***/

	END_TEST
}

void VectorSize()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	passed = v0.Size() == 0 && v0.Capacity() == 10;

	/*** END TEST ***/

	END_TEST
}

void VectorCapacity()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	passed = v0.Size() == 0;

	/*** END TEST ***/

	END_TEST
}

void VectorData()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);

	passed = v0.Data();

	/*** END TEST ***/

	END_TEST
}

void VectorSubscript()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0(10);
	for (I32 i = 1; i <= 10; ++i) { v0.Push(i); }

	passed = v0[4] == 5;

	/*** END TEST ***/

	END_TEST
}

void VectorPushSpeed()
{
	BEGIN_TEST;

	/*** START TEST ***/

	Vector<I32> v0;
	for (I32 i = 0; i < 1000000; ++i)
	{
		v0.Push(i);
	}

	/*** END TEST ***/

	END_TEST
}

void STLVectorPushSpeed()
{
	BEGIN_TEST;

	/*** START TEST ***/

	std::vector<I32> v0;
	for (I32 i = 0; i < 1000000; ++i)
	{
		v0.push_back(i);
	}

	/*** END TEST ***/

	END_TEST
}
#pragma endregion

#pragma region String Tests

void StringInit_Blank()
{
	BEGIN_TEST;

	String str0;
	String8 str1;
	String16 str2;
	String32 str3;
	StringW str4;

	passed = str0.Size() == 0 && str0.Capacity() == 0 && str0.Data() == nullptr &&
		str1.Size() == 0 && str1.Capacity() == 0 && str1.Data() == nullptr &&
		str2.Size() == 0 && str2.Capacity() == 0 && str2.Data() == nullptr &&
		str3.Size() == 0 && str3.Capacity() == 0 && str3.Data() == nullptr &&
		str4.Size() == 0 && str4.Capacity() == 0 && str4.Data() == nullptr;

	END_TEST;
}

void StringInit_Array()
{
	CH* a0 = new CH[10];
	a0[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a0[i] = i + 100; }
	C8* a1 = new C8[10];
	a1[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a1[i] = i + 100; }
	C16* a2 = new C16[10];
	a2[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a2[i] = i + 100; }
	C32* a3 = new C32[10];
	a3[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a3[i] = i + 100; }
	CW* a4 = new CW[10];
	a4[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a4[i] = i + 100; }

	BEGIN_TEST;

	String str0(a0);
	String8 str1(a1);
	String16 str2(a2);
	String32 str3(a3);
	StringW str4(a4);

	passed = str0.Size() == 9 && str0.Capacity() == 1024 && Compare(str0.Data(), "defghijkl", 9) &&
		str1.Size() == 9 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"defghijkl", 9) &&
		str2.Size() == 9 && str2.Capacity() == 1024 && Compare(str2.Data(), u"defghijkl", 9) &&
		str3.Size() == 9 && str3.Capacity() == 1024 && Compare(str3.Data(), U"defghijkl", 9) &&
		str4.Size() == 9 && str4.Capacity() == 1024 && Compare(str4.Data(), L"defghijkl", 9);

	END_TEST;

	delete[] a0;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
}

void StringInit_ArrayLength()
{
	CH* a0 = new CH[10];
	a0[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a0[i] = i + 100; }
	C8* a1 = new C8[10];
	a1[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a1[i] = i + 100; }
	C16* a2 = new C16[10];
	a2[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a2[i] = i + 100; }
	C32* a3 = new C32[10];
	a3[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a3[i] = i + 100; }
	CW* a4 = new CW[10];
	a4[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a4[i] = i + 100; }

	BEGIN_TEST;

	String str0(a0, 5);
	String8 str1(a1, 5);
	String16 str2(a2, 5);
	String32 str3(a3, 5);
	StringW str4(a4, 5);

	passed = str0.Size() == 5 && str0.Capacity() == 1024 && Compare(str0.Data(), "defgh", 5) &&
		str1.Size() == 5 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"defgh", 5) &&
		str2.Size() == 5 && str2.Capacity() == 1024 && Compare(str2.Data(), u"defgh", 5) &&
		str3.Size() == 5 && str3.Capacity() == 1024 && Compare(str3.Data(), U"defgh", 5) &&
		str4.Size() == 5 && str4.Capacity() == 1024 && Compare(str4.Data(), L"defgh", 5);

	END_TEST;

	delete[] a0;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
}

void StringInit_Literal()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"zß水🍌𐐷Ͼ");
	String32 str3(U"👩‍🦰👨‍🦲zß水🍌");
	StringW str4(L"zß水🍌𐐷Ͼ");

	passed = str0.Size() == 13 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello, World!", 13) &&
		str1.Size() == 13 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello, World!", 13) &&
		str2.Size() == 8 && str2.Capacity() == 1024 && Compare(str2.Data(), u"zß水🍌𐐷Ͼ", 8) &&
		str3.Size() == 10 && str3.Capacity() == 1024 && Compare(str3.Data(), U"👩‍🦰👨‍🦲zß水🍌", 10) &&
		str4.Size() == 8 && str4.Capacity() == 1024 && Compare(str4.Data(), L"zß水🍌𐐷Ͼ", 8);

	END_TEST;
}

void StringInit_Copy()
{
	BEGIN_TEST;

	String c0("Hello, World!");
	String8 c1(u8"Hello, World!");
	String16 c2(u"Hello, World!");
	String32 c3(U"Hello, World!");
	StringW c4(L"Hello, World!");

	String str0(c0);
	String8 str1(c1);
	String16 str2(c2);
	String32 str3(c3);
	StringW str4(c4);

	passed = c0.Size() == 13 && c0.Capacity() == 1024 && Compare(c0.Data(), "Hello, World!", 13) &&
		c1.Size() == 13 && c1.Capacity() == 1024 && Compare(c1.Data(), u8"Hello, World!", 13) &&
		c2.Size() == 13 && c2.Capacity() == 1024 && Compare(c2.Data(), u"Hello, World!", 13) &&
		c3.Size() == 13 && c3.Capacity() == 1024 && Compare(c3.Data(), U"Hello, World!", 13) &&
		c4.Size() == 13 && c4.Capacity() == 1024 && Compare(c4.Data(), L"Hello, World!", 13) &&
		str0.Size() == 13 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello, World!", 13) &&
		str1.Size() == 13 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello, World!", 13) &&
		str2.Size() == 13 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Hello, World!", 13) &&
		str3.Size() == 13 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Hello, World!", 13) &&
		str4.Size() == 13 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Hello, World!", 13) &&
		c0.Data() != str0.Data() && c1.Data() != str1.Data() && c2.Data() != str2.Data() &&
		c3.Data() != str3.Data() && c4.Data() != str4.Data();

	END_TEST;
}

void StringInit_Move()
{
	BEGIN_TEST;

	String c0("Hello, World!");
	String8 c1(u8"Hello, World!");
	String16 c2(u"Hello, World!");
	String32 c3(U"Hello, World!");
	StringW c4(L"Hello, World!");

	String str0(Move(c0));
	String8 str1(Move(c1));
	String16 str2(Move(c2));
	String32 str3(Move(c3));
	StringW str4(Move(c4));

	passed = c0.Size() == 0 && c0.Capacity() == 0 && c0.Data() == nullptr &&
		c1.Size() == 0 && c1.Capacity() == 0 && c1.Data() == nullptr &&
		c2.Size() == 0 && c2.Capacity() == 0 && c2.Data() == nullptr &&
		c3.Size() == 0 && c3.Capacity() == 0 && c3.Data() == nullptr &&
		c4.Size() == 0 && c4.Capacity() == 0 && c4.Data() == nullptr &&
		str0.Size() == 13 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello, World!", 13) &&
		str1.Size() == 13 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello, World!", 13) &&
		str2.Size() == 13 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Hello, World!", 13) &&
		str3.Size() == 13 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Hello, World!", 13) &&
		str4.Size() == 13 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Hello, World!", 13);

	END_TEST;
}

void StringAssign_NullPointer()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0 = nullptr;
	str1 = nullptr;
	str2 = nullptr;
	str3 = nullptr;
	str4 = nullptr;

	passed = str0.Size() == 0 && str0.Capacity() == 0 && str0.Data() == nullptr &&
		str1.Size() == 0 && str1.Capacity() == 0 && str1.Data() == nullptr &&
		str2.Size() == 0 && str2.Capacity() == 0 && str2.Data() == nullptr &&
		str3.Size() == 0 && str3.Capacity() == 0 && str3.Data() == nullptr &&
		str4.Size() == 0 && str4.Capacity() == 0 && str4.Data() == nullptr;

	END_TEST;
}

void StringAssign_Array()
{
	CH* a0 = new CH[10];
	a0[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a0[i] = i + 100; }
	C8* a1 = new C8[10];
	a1[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a1[i] = i + 100; }
	C16* a2 = new C16[10];
	a2[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a2[i] = i + 100; }
	C32* a3 = new C32[10];
	a3[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a3[i] = i + 100; }
	CW* a4 = new CW[10];
	a4[9] = '\0';
	for (U32 i = 0; i < 9; ++i) { a4[i] = i + 100; }

	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0 = a0;
	str1 = a1;
	str2 = a2;
	str3 = a3;
	str4 = a4;

	passed = str0.Size() == 9 && str0.Capacity() == 1024 && Compare(str0.Data(), "defghijkl", 9) &&
		str1.Size() == 9 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"defghijkl", 9) &&
		str2.Size() == 9 && str2.Capacity() == 1024 && Compare(str2.Data(), u"defghijkl", 9) &&
		str3.Size() == 9 && str3.Capacity() == 1024 && Compare(str3.Data(), U"defghijkl", 9) &&
		str4.Size() == 9 && str4.Capacity() == 1024 && Compare(str4.Data(), L"defghijkl", 9);

	END_TEST;

	delete[] a0;
	delete[] a1;
	delete[] a2;
	delete[] a3;
	delete[] a4;
}

void StringAssign_Literal()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0 = "Goodbye, World!";
	str1 = u8"Goodbye, World!";
	str2 = u"zß水🍌𐐷Ͼ";
	str3 = U"👩‍🦰👨‍🦲zß水🍌";
	str4 = L"zß水🍌𐐷Ͼ";

	passed = str0.Size() == 15 && str0.Capacity() == 1024 && Compare(str0.Data(), "Goodbye, World!", 15) &&
		str1.Size() == 15 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Goodbye, World!", 15) &&
		str2.Size() == 8 && str2.Capacity() == 1024 && Compare(str2.Data(), u"zß水🍌𐐷Ͼ", 8) &&
		str3.Size() == 10 && str3.Capacity() == 1024 && Compare(str3.Data(), U"👩‍🦰👨‍🦲zß水🍌", 10) &&
		str4.Size() == 8 && str4.Capacity() == 1024 && Compare(str4.Data(), L"zß水🍌𐐷Ͼ", 8);

	END_TEST;
}

void StringAssign_Copy()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	String str5("Goodbye, World!");
	String8 str6(u8"Goodbye, World!");
	String16 str7(u"Goodbye, World!");
	String32 str8(U"Goodbye, World!");
	StringW str9(L"Goodbye, World!");

	str0 = str5;
	str1 = str6;
	str2 = str7;
	str3 = str8;
	str4 = str9;

	//TODO: Somehow confirm old ptr was freed

	passed = str0.Size() == 15 && str0.Capacity() == 1024 && Compare(str0.Data(), "Goodbye, World!", 15) &&
		str1.Size() == 15 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Goodbye, World!", 15) &&
		str2.Size() == 15 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Goodbye, World!", 15) &&
		str3.Size() == 15 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Goodbye, World!", 15) &&
		str4.Size() == 15 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Goodbye, World!", 15) &&
		str5.Size() == 15 && str5.Capacity() == 1024 && Compare(str5.Data(), "Goodbye, World!", 15) &&
		str6.Size() == 15 && str6.Capacity() == 1024 && Compare(str6.Data(), u8"Goodbye, World!", 15) &&
		str7.Size() == 15 && str7.Capacity() == 1024 && Compare(str7.Data(), u"Goodbye, World!", 15) &&
		str8.Size() == 15 && str8.Capacity() == 1024 && Compare(str8.Data(), U"Goodbye, World!", 15) &&
		str9.Size() == 15 && str9.Capacity() == 1024 && Compare(str9.Data(), L"Goodbye, World!", 15);

	END_TEST;
}

void StringAssign_CopySelf()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	CH* p0 = str0.Data();
	C8* p1 = str1.Data();
	C16* p2 = str2.Data();
	C32* p3 = str3.Data();
	CW* p4 = str4.Data();

	str0 = str0;
	str1 = str1;
	str2 = str2;
	str3 = str3;
	str4 = str4;

	passed = str0.Data() == p0 && str1.Data() == p1 && str2.Data() == p2 && 
		str3.Data() == p3 && str4.Data() == p4 && 
		str0.Size() == 13 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello, World!", 13) &&
		str1.Size() == 13 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello, World!", 13) &&
		str2.Size() == 13 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Hello, World!", 13) &&
		str3.Size() == 13 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Hello, World!", 13) &&
		str4.Size() == 13 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Hello, World!", 13);

	END_TEST;
}

void StringAssign_Move()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	String str5("Goodbye, World!");
	String8 str6(u8"Goodbye, World!");
	String16 str7(u"Goodbye, World!");
	String32 str8(U"Goodbye, World!");
	StringW str9(L"Goodbye, World!");

	str0 = Move(str5);
	str1 = Move(str6);
	str2 = Move(str7);
	str3 = Move(str8);
	str4 = Move(str9);

	//TODO: Somehow confirm old ptr was freed

	passed = str0.Size() == 15 && str0.Capacity() == 1024 && Compare(str0.Data(), "Goodbye, World!", 15) &&
		str1.Size() == 15 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Goodbye, World!", 15) &&
		str2.Size() == 15 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Goodbye, World!", 15) &&
		str3.Size() == 15 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Goodbye, World!", 15) &&
		str4.Size() == 15 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Goodbye, World!", 15) &&
		str5.Size() == 0 && str5.Capacity() == 0 && str5.Data() == nullptr &&
		str6.Size() == 0 && str6.Capacity() == 0 && str6.Data() == nullptr &&
		str7.Size() == 0 && str7.Capacity() == 0 && str7.Data() == nullptr &&
		str8.Size() == 0 && str8.Capacity() == 0 && str8.Data() == nullptr &&
		str9.Size() == 0 && str9.Capacity() == 0 && str9.Data() == nullptr;

	END_TEST;
}

void StringAssign_MoveSelf()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	CH* p0 = str0.Data();
	C8* p1 = str1.Data();
	C16* p2 = str2.Data();
	C32* p3 = str3.Data();
	CW* p4 = str4.Data();

	str0 = Move(str0);
	str1 = Move(str1);
	str2 = Move(str2);
	str3 = Move(str3);
	str4 = Move(str4);

	passed = str0.Data() == p0 && str1.Data() == p1 && str2.Data() == p2 &&
		str3.Data() == p3 && str4.Data() == p4 &&
		str0.Size() == 13 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello, World!", 13) &&
		str1.Size() == 13 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello, World!", 13) &&
		str2.Size() == 13 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Hello, World!", 13) &&
		str3.Size() == 13 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Hello, World!", 13) &&
		str4.Size() == 13 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Hello, World!", 13);

	END_TEST;
}

void StringDestroy()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0.Destroy();
	str1.Destroy();
	str2.Destroy();
	str3.Destroy();
	str4.Destroy();

	passed = str0.Size() == 0 && str0.Capacity() == 0 && str0.Data() == nullptr &&
		str1.Size() == 0 && str1.Capacity() == 0 && str1.Data() == nullptr &&
		str2.Size() == 0 && str2.Capacity() == 0 && str2.Data() == nullptr &&
		str3.Size() == 0 && str3.Capacity() == 0 && str3.Data() == nullptr &&
		str4.Size() == 0 && str4.Capacity() == 0 && str4.Data() == nullptr;

	END_TEST;
}

void StringClear()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0.Clear();
	str1.Clear();
	str2.Clear();
	str3.Clear();
	str4.Clear();

	passed = str0.Size() == 0 && str0.Capacity() == 1024 && Compare(str0.Data(), "\0ello, World!", 13) &&
		str1.Size() == 0 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"\0ello, World!", 13) &&
		str2.Size() == 0 && str2.Capacity() == 1024 && Compare(str2.Data(), u"\0ello, World!", 13) &&
		str3.Size() == 0 && str3.Capacity() == 1024 && Compare(str3.Data(), U"\0ello, World!", 13) &&
		str4.Size() == 0 && str4.Capacity() == 1024 && Compare(str4.Data(), L"\0ello, World!", 13);

	END_TEST;
}

void StringResize()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");

	str0.Data()[5] = '\0';
	str1.Data()[5] = u8'\0';
	str2.Data()[5] = u'\0';
	str3.Data()[5] = U'\0';
	str4.Data()[5] = L'\0';

	str0.Resize();
	str1.Resize();
	str2.Resize();
	str3.Resize();
	str4.Resize();

	passed = str0.Size() == 5 && str0.Capacity() == 1024 && Compare(str0.Data(), "Hello", 5) &&
		str1.Size() == 5 && str1.Capacity() == 1024 && Compare(str1.Data(), u8"Hello", 5) &&
		str2.Size() == 5 && str2.Capacity() == 1024 && Compare(str2.Data(), u"Hello", 5) &&
		str3.Size() == 5 && str3.Capacity() == 1024 && Compare(str3.Data(), U"Hello", 5) &&
		str4.Size() == 5 && str4.Capacity() == 1024 && Compare(str4.Data(), L"Hello", 5);

	END_TEST;
}

//TODO: Reserve

void StringBlank()
{
	BEGIN_TEST;

	String str0("Hello, World!");
	String8 str1(u8"Hello, World!");
	String16 str2(u"Hello, World!");
	String32 str3(U"Hello, World!");
	StringW str4(L"Hello, World!");
	String str5;
	String8 str6;
	String16 str7;
	String32 str8;
	StringW str9;
	String str10("\n\r\t\v\f		");
	String8 str11(u8"\n\r\t\v\f		");
	String16 str12(u"\n\r\t\v\f		");
	String32 str13(U"\n\r\t\v\f		");
	StringW str14(L"\n\r\t\v\f		");

	passed = !str0.Blank() && !str1.Blank() && !str2.Blank() && !str3.Blank() && !str4.Blank() &&
		str5.Blank() && str6.Blank() && str7.Blank() && str8.Blank() && str9.Blank() &&
		str10.Blank() && str11.Blank() && str12.Blank() && str13.Blank() && str14.Blank();

	END_TEST;
}

#pragma endregion

int main()
{
	QueryPerformanceFrequency(&freq);

#pragma region String Tests
	printf("STRING TESTS: \n");
	StringInit_Blank();
	StringInit_Array();
	StringInit_ArrayLength();
	StringInit_Literal();
	StringInit_Copy();
	StringInit_Move();
	StringAssign_NullPointer();
	StringAssign_Array();
	StringAssign_Literal();
	StringAssign_Copy();
	StringAssign_CopySelf();
	StringAssign_Move();
	StringAssign_MoveSelf();
	StringDestroy();
	StringClear();
	StringResize();

	StringBlank();
#pragma endregion

#pragma region Vector Tests
	printf("\nVECTOR TESTS: \n");
	VectorInit_Blank();
	VectorInit_Capacity();
	VectorInit_Size();
	VectorInit_Copy();
	VectorInit_Move();
	VectorInit_Initializer();
	VectorAssign_Copy();
	VectorAssign_Move();
	VectorAssign_Initializer();
	VectorDestroy();
	VectorNewDelete();
	VectorPush_Copy();
	VectorPush_Move();
	VectorPop();
	VectorPop_Copy();
	VectorPop_Move();
	VectorInsert_Copy();
	VectorInsert_Move();
	VectorInsert_CopyVector();
	VectorInsert_MoveVector();
	VectorRemove();
	VectorRemove_Copy();
	VectorRemove_Move();
	VectorErase();
	VectorErase_Copy();
	VectorSplit();
	VectorMerge_Copy();
	VectorMerge_Move();
	VectorAdd_Copy();
	VectorAdd_Move();
	VectorPredicate_SearchFor();
	VectorPredicate_SearchForIndices();
	VectorPredicate_SearchCount();
	VectorPredicate_RemoveAll();
	VectorPredicate_RemoveAll_Other();
	VectorReserve();
	VectorResize();
	VectorResize_Value();
	VectorShrink();
	VectorClear();
	VectorContains();
	VectorCount();
	VectorFind();
	VectorSize();
	VectorCapacity();
	VectorData();
	VectorSubscript();
	VectorPushSpeed();
	STLVectorPushSpeed();
#pragma endregion
}
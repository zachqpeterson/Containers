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
bool passed = true;				\
LARGE_INTEGER start;			\
QueryPerformanceCounter(&start);

constexpr int Length(const char* str)
{
	return *str ? 1 + Length(str + 1) : 0;
}

#define END_TEST																		\
LARGE_INTEGER end;																		\
QueryPerformanceCounter(&end);															\
F64 time = (F64)(end.QuadPart - start.QuadPart) / (F64)freq.QuadPart;					\
constexpr char len = Length(__FUNCTION__);												\
char name[len + 2] = __FUNCTION__;														\
name[len] = ':';																		\
name[len + 1] = '\0';																	\
printf("%-35s %s  |  Time taken: %fs\n", name, passed ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m", time);

LARGE_INTEGER freq;

bool Pred0(U64 i) { return i == 2; }

bool Pred1(U64 i) { return i < 5; }

#pragma region VectorTests
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

	I32 cap = v0.Capacity();

	for (I32 i = 10; i > 0; --i)
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

	I32 cap = v0.Capacity();

	for (I32 i = 10; i > 0; --i)
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

	I32 cap = v0.Capacity();

	for (I32 i = 10; i > 0; --i)
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

int main()
{
	QueryPerformanceFrequency(&freq);

	const String str("Hello, World");

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
}
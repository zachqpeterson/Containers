#include "Vector.h"
#include "List.h"
#include "Initiazer.h"
#include "Defines.h"

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
printf("%-30s %s  |  Time taken: %fs\n", name, passed ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m", time);

LARGE_INTEGER freq;

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

	passed = false;

	/*** END TEST ***/

	END_TEST
}

void VectorPush_Move()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorPop()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorPop_Copy()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorPop_Move()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorInsert_Copy()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorInsert_Move()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorInsert_CopyVector()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorInsert_MoveVector()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorRemove()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorSplit()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorMerge_Copy()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorMerge_Move()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorAdd_Copy()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorAdd_Move()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorReserve()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorResize()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorResize_Value()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorClear()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorContains()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorCount()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorFind()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorSize()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorCapacity()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorData()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}

void VectorSubscript()
{
	BEGIN_TEST

		/*** START TEST ***/



		/*** END TEST ***/

		END_TEST
}
#pragma endregion

int main()
{
	QueryPerformanceFrequency(&freq);

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
	VectorSplit();
	VectorMerge_Copy();
	VectorMerge_Move();
	VectorAdd_Copy();
	VectorAdd_Move();
	VectorReserve();
	VectorResize();
	VectorResize_Value();
	VectorClear();
	VectorContains();
	VectorCount();
	VectorFind();
	VectorSize();
	VectorCapacity();
	VectorData();
	VectorSubscript();
}
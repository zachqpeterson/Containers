#include "Vector.h"
#include "List.h"
#include "Initiazer.h"

#include <Windows.h>
#include <stdio.h>
#include <vector>

int main()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	/*** INIT TEST ***/

	List<int> v;
	v.PushBack(0);
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);

	/*** END INIT TEST ***/

	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);

	/*** START TEST ***/

	v.Remove(3);

	/*** END TEST ***/

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	/*** VALIDATE TEST ***/

	for (int i : v) { printf("%d\n", i); }

	/*** END VALIDATE TEST ***/

	U64 time = end.QuadPart - start.QuadPart;

	printf("%f", time / (float)freq.QuadPart);
}
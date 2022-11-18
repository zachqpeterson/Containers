#include "Vector.h"
#include "List.h"
#include "Initiazer.h"

#include <Windows.h>
#include <stdio.h>

int main()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	/*** INIT TEST ***/

	Vector<int> v;

	/*** END INIT TEST ***/

	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);

	/*** START TEST ***/

	for (int i = 0; i < 100000; ++i) { v.Insert(i, i); }

	/*** END TEST ***/

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	/*** VALIDATE TEST ***/



	/*** END VALIDATE TEST ***/

	U64 time = end.QuadPart - start.QuadPart;

	printf("%f", time / (float)freq.QuadPart);
}
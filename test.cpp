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

	Vector<int> v{ 5, 5 };
	Vector<int> v2{ 5, 3 };

	/*** END INIT TEST ***/

	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);

	/*** START TEST ***/

	v += Move(v2);

	/*** END TEST ***/

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	/*** VALIDATE TEST ***/

	for (int i : v) { printf("%d\n", i); }

	/*** END VALIDATE TEST ***/

	U64 time = end.QuadPart - start.QuadPart;

	printf("%f", time / (float)freq.QuadPart);
}
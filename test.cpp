#include "Vector.h"
#include "List.h"
#include "Initiazer.h"

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <list>

int main()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	/*** INIT TEST ***/



	/*** END INIT TEST ***/

	LARGE_INTEGER start;
	QueryPerformanceCounter(&start);

	/*** START TEST ***/



	/*** END TEST ***/

	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);

	/*** VALIDATE TEST ***/



	/*** END VALIDATE TEST ***/

	U64 time = end.QuadPart - start.QuadPart;

	printf("%f", time / (float)freq.QuadPart);
}
#include "environment.h"

ENTRYPOINT INT32 main(VOID)
{
	// USIZE doubleSize = sizeof(DOUBLE);
	// PrintFormatedString("Size of DOUBLE: %u bytes\r\n", doubleSize);
	for (UINT64 i = 0; i < 10000000; i++)
	{
		PrintFormatedString("%d-%d-%d-%d\r\n", i, i, i, i);
	}

	ExitProcess(0);
}

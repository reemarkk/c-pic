#include "environment.h"
#include "console.h"

ENTRYPOINT DOUBLE _start(VOID)
{
#if defined(PLATFORM_WINDOWS_I386)

	PCHAR currentAddress = GetInstructionAddress(); // Get the return address of the caller function

	UINT16 functionPrologue = 0x8955; // i386 function prologue: push ebp; mov ebp, esp

	PCHAR functionStart = ReversePatternSearch(currentAddress, (PCHAR)&functionPrologue, sizeof(functionPrologue)); // Scan backward for function prologue

	SetEnvironmentBaseAddress(functionStart); // Set the environment base address to the start of the function
#endif

	DOUBLE val = MAKE_DOUBLE(12345.6789);

	PrintFormatedString(UTF8("Example DOUBLE: %f\n"), val);

	ExitProcess(0);
}
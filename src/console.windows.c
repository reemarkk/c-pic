#include "console.h"
#include "environment.h"

#if defined(PLATFORM_WINDOWS)

// Function pointer type for WriteConsoleA_t function
typedef BOOL(WINAPI* WriteConsoleA)(PVOID hConsoleOutput, const PCHAR lpBuffer, UINT32 nNumberOfCharsToWrite, PUINT32 lpNumberOfCharsWritten, PUINT32 lpReserved);

UINT32 WriteConsole(const PCHAR output,  USIZE outputLength){
	PPEB peb = GetCurrentPEB(); // Get the current process's PEB pointer

	// Name of the module to resolve
	PWCHAR moduleName =  L"Kernel32.dll";
	// Resolve the module handle
	PVOID kernel32Base = GetModuleHandleFromPEB(peb, moduleName);
	// Validate the module handle
	if (kernel32Base == NULL)
		return -1;
	// Name of the function to resolve
	PCHAR functionName = "WriteConsoleA";
	// Resolve the function address
	WriteConsoleA writeConsoleA= (WriteConsoleA)GetExportAddress(kernel32Base, functionName);
	// Validate the function address
	if (writeConsoleA == NULL)
		return -2;
		
	UINT32 numberOfCharsWritten = 0;
	// Call the WriteConsoleA function
	writeConsoleA(peb->ProcessParameters->StandardOutput, output, outputLength, &numberOfCharsWritten, NULL);

	return numberOfCharsWritten;
}

#endif
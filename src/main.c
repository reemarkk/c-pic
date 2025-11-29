#include "environment.h"
#include "pe.h"

ENTRYPOINT INT32 main() {
	
	PCHAR message = "Hello world!\n";

	USIZE messageLength = GetStringLength(message);
	WriteConsole(message, messageLength);

	ExitProcess(0);
}


#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "primitives.h"

#include "console.h"
#include "string.h"

#if defined(PLATFORM_WINDOWS)

	#include "peb.h"
	#include "pe.h"

	// This is just a placeholder macro for Windows platform.
	// The ExitProcess macro must be called only in the entry point function to terminate the process; otherwise, it will simply return from other functions.
	#define ExitProcess(code)  return code

#elif defined(PLATFORM_LINUX)
	
	NO_RETURN VOID ExitProcess(USIZE code); 

#endif

#endif // __ENVIRONMENT_H__

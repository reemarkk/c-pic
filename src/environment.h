#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#if !defined(__clang__)
	#error "This code must be compiled with Clang!"
#endif

#include "primitives.h"

#include "console.h"
#include "string.h"

#define SECTION(x)  

#if defined(PLATFORM_WINDOWS)

	#include "peb.h"
	#include "pe.h"
	
	#define ENTRYPOINT __attribute__((section(".text$main")))

#if defined(PLATFORM_WINDOWS_I386)
	#define WINAPI  __attribute__((stdcall))
#elif defined(PLATFORM_WINDOWS_AMD64) 
	#define WINAPI  __attribute__((ms_abi))
#elif defined(PLATFORM_WINDOWS_ARM32)
	#define WINAPI	
#elif defined(PLATFORM_WINDOWS_ARM64)
	#define WINAPI  
#endif

	#define ExitProcess(code)  return code

#elif defined(PLATFORM_LINUX)

	#define ENTRYPOINT __attribute__((section(".text$main"))) __attribute__((noreturn))
	#define ExitProcess(code)  TerminateProcess(code)

	VOID TerminateProcess(USIZE code); 

#endif



#endif // __ENVIRONMENT_H__

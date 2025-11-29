
#include "console.h"

#if defined(PLATFORM_LINUX)

UINT32 WriteConsole(const PCHAR output,  USIZE outputLength) {

#if defined(PLATFORM_LINUX_AMD64)
    register const CHAR *rsi asm("rsi") = output;
    register INT64 rdx asm("rdx") = outputLength;
    register INT64 rdi asm("rdi") = 1;
    register INT64 rax asm("rax") = 1;

    __asm__ volatile(
        "syscall"
        : "+r"(rax)
        : "r"(rdi), "r"(rsi), "r"(rdx)
        : "rcx", "r11", "memory", "cc"
    );
#elif defined(PLATFORM_LINUX_ARM64)
	register INT64 x0 __asm__("x0") = 1;      // fd = 1 (stdout)
    register const CHAR *x1 __asm__("x1") = output;
    register INT64 x2 __asm__("x2") = outputLength;
    register INT64 x8 __asm__("x8") = 64;     // SYS_write on aarch64

    __asm__ volatile(
        "svc 0"
        : "+r"(x0)                // x0 = return value (ssize_t)
        : "r"(x1), "r"(x2), "r"(x8)
        : "memory", "cc"
    );
	#elif defined(PLATFORM_LINUX_I386)
	__asm__ volatile(
        "movl $4, %%eax\n\t"    /* SYS_write = 4 */
        "movl $1, %%ebx\n\t"    /* fd = stdout */
        "movl %0, %%ecx\n\t"    /* buffer */
        "movl %1, %%edx\n\t"    /* length */
        "int $0x80"
        :
        : "r"(output), "r"(outputLength)
        : "eax", "ebx", "ecx", "edx", "memory", "cc"
    );
#endif

    return (UINT32)outputLength;
}

#endif
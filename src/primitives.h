#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#define TRUE    ((BOOL)1)
#define FALSE   ((BOOL)0)
#define towlower(c) (((c) >= L'A' && (c) <= L'Z') ? ((c) + (L'a' - L'A')) : (c))
#define tolower(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + ('a' - 'A')) : (c))
#define NULL ((PVOID)0)

typedef void VOID, * PVOID,** PPVOID;

typedef signed char INT8, * PINT8;
typedef unsigned char UINT8, * PUINT8, ** PPUINT8;

typedef signed short int INT16, * PINT16;
typedef unsigned short UINT16, * PUINT16;

typedef signed int INT32, * PINT32;
typedef unsigned int UINT32, * PUINT32, ** PPUINT32;

typedef signed long long int INT64, * PINT64;
typedef unsigned long long int UINT64, * PUINT64,** PPUINT64;

typedef UINT8 BOOL, * PBOOL,** PPBOOL;

typedef char CHAR, * PCHAR, ** PPCHAR;
typedef unsigned char UCHAR, * PUCHAR;

#if defined(PLATFORM_WINDOWS)
    typedef UINT16 WCHAR, * PWCHAR, ** PPWCHAR;
#elif defined(PLATFORM_LINUX)
    typedef INT32 WCHAR, * PWCHAR, ** PPWCHAR;
#else
    #error Unsupported platform
#endif

#if defined(ARCHITECTURE_AMD64) || defined(ARCHITECTURE_ARM64)
    typedef UINT64 USIZE , * PUSIZE;
    typedef INT64 SSIZE , * PSSIZE;
#else
    typedef UINT32 USIZE , * PUSIZE;
    typedef INT32 SSIZE , * PSSIZE;
#endif 

#endif // __PRIMITIVES_H__
